/*************************************************************************
	> File Name: fuwuqi.c
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 16时53分05秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
/*服务器监听的本地端口*/
#define MYPORT 4000
/*能够同时接受多少没有accept的连接*/
#define BACKLOG 10
void handler(int sig)
{
    if(sig == SIGUSR1)
    {
        exit(0);
    }
}

int main()
{
    signal(SIGUSR1, handler);      //杀死子进程
    int sockfd, new_fd, numbytes;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    int len;

    if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero( &(my_addr.sin_zero), 8);

    int on = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    perror("setsockopt");

    if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    while(1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if((new_fd = accept(sockfd,(struct sockaddr*)&their_addr, &sin_size))== -1)
        {
            //perror("accept");
            continue;
        }
        printf("server: got connection fron %s\n", inet_ntoa(their_addr.sin_addr));

        pid_t id = fork();
        if(id < 0)
        {
            perror("fork");
            exit(1);
        }
        if(id > 0)
        {
            close(sockfd);
            char recvbuf[1024];
            while(1)
            {
                memset(recvbuf, 0, sizeof(recvbuf));
                numbytes = recv(new_fd, recvbuf, sizeof(recvbuf),0);
                if(recvbuf[0] == '2')
                {   
                    //close(new_fd);     //当客户端发"2"时, 子进程关闭newfd 父进程也关闭newfd  客户端用直接关闭(因为不会阻塞)
                    //close(new_fd);     //连续执行两次 并不能起到同上的作用

                    //shutdown(new_fd, SHUT_RD);   //双方都不退出, 客户端仍然可以写(写向哪里?),服务器读关闭,不再读取客户发送的信息
                    shutdown(new_fd, SHUT_WR);   //双反都关闭, 服务器直接退出窗口, 但是还在监听. 客户端read到0 真正退出
                    //shutdown(new_fd, SHUT_RDWR);
                
                }
                if(numbytes == 0)
                {
                    printf("对方已经关闭\n");
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
                send(new_fd, recvbuf, sizeof(recvbuf), 0);
            }
            close(new_fd);
            kill(id, SIGUSR1);
        }
        if(id == 0)
        {
           close(new_fd);
        }
    }
close(sockfd);
return 0;
}

