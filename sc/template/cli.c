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
#include<sys/wait.h>
#include<errno.h>
#include<string.h>
#define PORT 8000
#define ERR_EXIT(x) \
    do \
    { \
        perror(x); \
        exit(1); \
    }while(0)      //分号的有无???

int main()
{
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
    printf("id = %s  port = %d", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port));

    //read
    //write
    
    return 0;
}
