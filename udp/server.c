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

//1.当udp程序在同一个端口启动两次, 那么第二次将无效
//2.如果设置了端口复用 ,
//  那么将重新绑定端口, 第一次的程序将无效(用了bind也一样)
//3.bind绑定端口, 且没用端口复用,第二次启动程序将被告知already in use
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

    int ret;
    int opt = 1;
    if ((ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) < 0)
    {
        perror("sersockopt error");
        exit(1);
    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);

    bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

    clientlen = sizeof(clientaddr);

    while(1)
    {
        //sendto 和 recvfrom  等价于 send 和 recv 多了两个参数,地址结构体和长度
        //sendmsg 和 recvmsg 只有3个参数, fd,特定结构体,flag
        recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&clientaddr, &clientlen);
        printf("client IP %s , PORT %d\n", inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ipstr, sizeof(clientlen)), ntohs(clientaddr.sin_port));
        printf("%s\n", buf);
        bzero(buf, sizeof(buf));
    }
    close(sockfd);
    return 0;
}
