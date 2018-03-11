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
    printf("RECV : %d", sig);
    exit(0);
}
void ech_cli(int sockfd)
{
    char sendbuf[1024];
    signal(SIGUSR1, handler);
    while(1)
    {
        memset(sendbuf, 0, sizeof(sendbuf));
        scanf("%s", sendbuf);
        send(sockfd, sendbuf, strlen(sendbuf), 0);

        char recvbuf[1024];
        memset(recvbuf, 0, sizeof(recvbuf));
        int ret = recv(sockfd, recvbuf, sizeof(recvbuf),0);
        if(ret== 0)
        {
            printf("对方已经关闭\n");
            //    kill(0, SIGUSR1); 
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
    int sockfd;

    struct hostent *he;
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

    ech_cli(sockfd);
    return 0;
}
