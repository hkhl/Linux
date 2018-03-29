/*************************************************************************
	> File Name: client.c
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
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("./client str\n");
        exit(1);
    }
    int confd, len;
    struct sockaddr_in serveraddr;
    char buf[MAXLINE];
    char ipstr[INET_ADDRSTRLEN] = "127.0.0.1";

    confd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipstr, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERVER_PORT);



        sendto(confd, argv[1], strlen(argv[1]), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));


    close(confd);
    return 0;
}
