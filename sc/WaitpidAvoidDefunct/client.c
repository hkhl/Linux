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

//测试一次性连接多个客户端,, 同时关闭, 而当客户端关闭的时候,服务器会给子进程发送信号SIGUSR1杀死子进程
//同时注册了信号signal(SIGCHLD, hand_sigchld).用来处理僵尸进程, 由于多个客户端同时关闭,所以同时发送多个不可靠信号 没有队列
//有的信号会未达,导致依然存在僵尸进程,所以要用while(1){waitpid(-1, NULL, WNOHANG) > 0}  NULL;
//
void handler(int sig)
{
    if(sig == SIGUSR1)
    {
        printf("RECV:  %d\n", sig);
        exit(0);
    }
}
void ech_cli(int sockfd)
{
   // pid_t id = fork();
   // if(id == 0)
    {
        char sendbuf[1024];
        while(1)
        {
            memset(sendbuf, 0, sizeof(sendbuf));
            scanf("%s", sendbuf);
            send(sockfd, sendbuf, strlen(sendbuf), 0);
        }
    }
   /* if(id > 0)
    {
        char recvbuf[1024];
        while(1)
        {
            memset(recvbuf, 0, sizeof(recvbuf));
            int ret = recv(sockfd, recvbuf, sizeof(recvbuf),0);
            if(ret== 0)
            {
                printf("对方已经关闭\n");
                kill(id, SIGUSR1);
                break;
            }
            else if(ret> 0)
            {
                printf("Received: %s\n",recvbuf);
            }
            else
            {
                perror("recv");
                exit(1);
            }
        } 
    }*/
    close(sockfd);
}


//一次建立5个连接
int main(int argc, char *argv[])
{
    signal(SIGUSR1, handler);
    int sock[10];

    for(int i = 0; i < 10; i++)
    {
        struct hostent *he;
        struct sockaddr_in their_addr;

        if((sock[i] = socket(AF_INET, SOCK_STREAM, 0))==-1)
        {
            perror("socket");
            exit(1);
        }
        their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons(PORT);
        their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        bzero(&(their_addr.sin_zero), 8);

        if(connect(sock[i], (struct sockaddr *)&their_addr,sizeof(struct sockaddr))==-1)
        {
            perror("connect");
            exit(1);
        }
    }
    ech_cli(sock[1]);
    return 0;
}
