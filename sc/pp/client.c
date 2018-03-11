/*************************************************************************
	> File Name: kehuduan.c
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 17时18分20秒
 ************************************************************************/
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<signal.h>
#include<unistd.h>
#define PORT 4000

void handler(int sig)
{
    if(sig == SIGUSR1)
    {
        printf("RECV:  %d\n", sig);
        exit(0);
    }
}
int main(int argc, char *argv[])
{

    signal(SIGUSR1, handler);
    int sockfd, numbytes;
    struct sockaddr_in their_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        perror("socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(their_addr.sin_zero), 8);

    if(connect(sockfd, (struct sockaddr *)&their_addr,sizeof(struct sockaddr))==-1)
    {
        perror("connect");
        exit(1);
    }


    //每次循环都创建了一个进程..
    //应该是先创建子进程, 父进程while(1)负责接受 子进程while(1)负责发送
    pid_t id = fork();
    if(id == 0)
    {
        char sendbuf[1024];
        while(1)
        {
            memset(sendbuf, 0, sizeof(sendbuf));
            scanf("%s", sendbuf);
            send(sockfd, sendbuf, strlen(sendbuf), 0);
        }
    }
    if(id > 0)
    {
        char recvbuf[1024];
        while(1)
        {
            memset(recvbuf, 0, sizeof(recvbuf));
            numbytes = recv(sockfd, recvbuf, sizeof(recvbuf),0);
            if(numbytes == 0)
            {
                printf("对方已经关闭\n");
                kill(id, SIGUSR1);  //关闭子进程
                break;
            }
            else if(numbytes > 0)
            {
                printf("Received: %s\n",recvbuf);
            }
            else
            {
                perror("recv");
                exit(1);
            }
        } 
    }
close(sockfd);

    return 0;
}
