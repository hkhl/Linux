/*************************************************************************
	> File Name: server.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年03月29日 星期四 15时58分24秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 1500
#define SERVER_PORT 8000 
int main(void)
{
    int sockfd;
    struct sockaddr_in serveraddr, clientaddr;
    char buf[MAXLINE];
    socklen_t clientlen;
    char ipstr[INET_ADDRSTRLEN];   //recv ipv4   长度16包括 '\0'
    //ipv6 -> INET6_ADDRStRLEN
    
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);
    
    bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

    clientlen = sizeof(clientaddr);

    while(1)
    {
        recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&clientaddr, &clientlen);

        printf("client IP %s , PORT %d\n", inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ipstr, sizeof(clientlen)), ntohs(clientaddr.sin_port));
        printf("%s\n", buf);
        bzero(buf, sizeof(buf));

    }
    close(sockfd);
    return 0;
}
