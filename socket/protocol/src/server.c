/*************************************************************************
	> File Name: srv.c
	> Author:
	> Mail:
	> Created Time: 2017年09月12日 星期二 20时04分38秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "msg.h"
#define PORT 8000
#define ERR_EXIT(x) \
    do \
    { \
        perror(x); \
        exit(1); \
    }while(0)


void do_service(int fd)          //回射
{
    /*和客户端进行双向通信*/
    char buff[512];
    while (1)
    {
        memset(buff, 0, sizeof(buff));
        int size;
        if ( (size = read_msg(fd, buff, sizeof(buff))) < 0)
        {
            perror("protocal error");
            break;
        }
        else if (size == 0)
            break;
        else
        {
            printf("size = %d\n", size);
            printf("%s\n", buff);
            //write(STDOUT_FILENO, buff, strlen(buff));
            if (write_msg(fd, buff, strlen(buff)) < 0)
            {
                if (errno == EPIPE) //对方关闭, 己方写管道破裂
                    break;
                perror("protocal error");
            }
        }
    }
}


//不用指针
void out_addr(struct sockaddr_in *clientaddr)
{
    //将端口转换为主机字节序
    int port = ntohs(clientaddr->sin_port);
    char ip[16];
    memset(ip, 0, sizeof(ip));
    //将ip从网络字节序转换成点分十进制
    inet_ntop(AF_INET, &clientaddr->sin_addr.s_addr, ip, sizeof(ip));

    printf("client: %s(%d)connected\n", ip, port);
}

void out_fd(int fd)
{
    struct sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);

    //从fd中获得连接的客户端的相关信息
    if (getpeername((fd), (struct sockaddr*)&peeraddr, &len) < 0)
    {
        perror("getpeername error");
        return ;
    }

    int port = ntohs(peeraddr.sin_port);
    char ip[16];
    memset(ip, 0, sizeof(ip));
    //将ip从网络字节序转换成点分十进制
    inet_ntop(AF_INET, &peeraddr.sin_addr.s_addr, ip, sizeof(ip));

    printf("client: %16s(%5d) closed\n", ip, port);
}


void* th_fn(void *arg)
{
    int fd = (int)arg;
    do_service(fd);
    out_fd(fd);
    close(fd);

    return (void *)0;
}


int main()
{

    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket");
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //地址复用
    int on = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");

    //bind
    if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr) ) < 0)
        ERR_EXIT("bind");

    //listen
    if(listen(sockfd,SOMAXCONN) < 0)
        ERR_EXIT("liten");

    //accept
    int connfd;
    struct sockaddr_in peeraddr;
    memset(&peeraddr, 0, sizeof(peeraddr));
    socklen_t len = sizeof(peeraddr);

    //设置线程分离属性
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    while (1)
    {
        if((connfd = accept(sockfd, (struct sockaddr *)&peeraddr, &len )) < 0)
        //if((connfd = accept(sockfd, NULL, NULL)) < 0)
        {
            perror("accept error");
            continue;
        }
        //打印客户信息         也可以不用设置结构体, accept参数给NULL
        //因为对用的客户信息在(connfd)中存在
        out_addr(&peeraddr);
        //printf("id = %s, port = %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

        /*
         * 启动子线程(线程分离)调用IO函数, 连接客户端双向通信
         */
        pthread_t th;
        int err;
        if ( (err = pthread_create(&th, &attr, th_fn, (void *)connfd)) != 0)
        {
            perror("pthread create error");
        }
        pthread_attr_destroy(&attr);
    }


    return 0;
}
