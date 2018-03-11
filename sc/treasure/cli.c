/*************************************************************************
	> File Name: cli.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月12日 星期二 20时33分22秒
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
#define PORT 8000
#include "sck.h"
int main()
{
    signal(SIGUSR1, handler);
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket");

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connect
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("connect");

    struct sockaddr_in localaddr;
    memset(&localaddr, 0, sizeof(localaddr));
    socklen_t len = sizeof(localaddr);
    if(getsockname(sockfd, (struct sockaddr *)&localaddr, &len) < 0)
        ERR_EXIT("getsockname");
    printf("ip = %s  port = %d\n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port));

    pid_t id = fork();

    if(id < 0)
        ERR_EXIT("fork");
    if(id == 0)
    {
        cs_send(sockfd);
    }
    if(id > 0)
    {
        int ret = cs_recv(sockfd);  //返回是0的时候对方关闭,, 需要关闭退出子进程
        if(ret == 0)
        {
            kill(id, SIGUSR1);
        }
    }
    close(sockfd);
    return 0;
}
