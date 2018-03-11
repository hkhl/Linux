/*************************************************************************
	> File Name: cli.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月31日 星期四 19时04分54秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/time.h>
//#include<netinet/in.h>
#define PORT 8000

void message(int sfd)
{
    char sendbuf[1024];
    char recvbuf[1024];

    fd_set rset;
    FD_ZERO(&rset);
    int nready;
    int maxfd;
    int fd_stdin = fileno(stdin);
    if(fd_stdin > sfd)
        maxfd = fd_stdin;
    else
        maxfd = sfd;

    while(1)
    {
        FD_SET(fd_stdin, &rset);
        FD_SET(sfd, &rset);
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        
        if(nready == -1)
        perror("select");
        if(nready == 0)
        continue;

        if(FD_ISSET(sfd, &rset))
        {
            int len;
            if((len = recv(sfd, recvbuf, sizeof(recvbuf), 0))== -1)
            {
                perror("recv");
            }
            else if(len == 0)
            {
                printf("server close\n");
                break;
            }
            else
            {
                puts(recvbuf);
            }
            memset(recvbuf, 0, sizeof(recvbuf));
        }

        if(FD_ISSET(fd_stdin, &rset))
        {
            fgets(sendbuf, sizeof(sendbuf), stdin);
            if(send(sfd, sendbuf, strlen(sendbuf), 0) == -1)
            {
                perror("send");
            }
            memset(sendbuf, 0, sizeof(sendbuf));
        }
    }
}

int main()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1)
    perror("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    perror("connect");

    message(sfd);

    close(sfd);
    return 0;
}
