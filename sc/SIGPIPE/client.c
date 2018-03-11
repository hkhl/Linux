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

//当杀死服务器的子进程时候, 客户端发送一条消息,自动关闭,
//和用了SIGPIPE的却别?  - -!
/*结论:  当服务器已经关闭, 客户端先被I/O阻塞, 先调用两次send函数将会被Linux内核发送信号SIGPIPE强制关闭*/
/*解决:  signal(SIGPIPE, SIG_IGN)*/
void ech_cli(int sockfd)
{
    while(1)
    {
        char sendbuf[1024];
        memset(sendbuf, 0, sizeof(sendbuf));
        scanf("%s", sendbuf);
        printf("111111\n");
        int r = send(sockfd, sendbuf, strlen(sendbuf), 0);
        if(r == -1)
        {
            perror("send1");
        }
        printf("222222\n");
        int re =send(sockfd, sendbuf, strlen(sendbuf), 0);
        if(re == -1)
        {
            perror("send2");
        }
        printf("333333\n");
        
        char recvbuf[1024];
        memset(recvbuf, 0, sizeof(recvbuf));
        int ret = recv(sockfd, recvbuf, sizeof(recvbuf),0);
        if(ret== 0)
        {
            printf("对方已经关闭\n");
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
    close(sockfd);
}


int main(int argc, char *argv[])
{
    int sock[10];
    //signal(SIGPIPE, SIG_IGN);

    for(int i = 0; i < 1; i++)
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
    ech_cli(sock[0]);
    return 0;
}
