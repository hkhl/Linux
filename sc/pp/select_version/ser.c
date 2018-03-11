/*************************************************************************
	> File Name: ser.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月31日 星期四 19时33分04秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#define PORT 8000
#define BACKLOG 10

void message(int cfd)
{
    char sendbuf[1024];
    char recvbuf[1024];

    fd_set rset;
    FD_ZERO(&rset);
    int nready;
    int maxfd;

    int fd_stdin = fileno(stdin);

    if(fd_stdin > cfd)
        maxfd = fd_stdin;
    else
        maxfd = cfd;

    while(1)
    {
        FD_SET(fd_stdin, &rset);
        FD_SET(cfd, &rset);
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        
        if(nready == -1)
        perror("select");
        if(nready == 0)
        continue;

        if(FD_ISSET(cfd, &rset))
        {
            int len;
            if((len = recv(cfd, recvbuf, sizeof(recvbuf), 0))== -1)
            {
                perror("recv");
            }
            else if(len == 0)
            {
                printf("peer close\n");
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
            if(send(cfd, sendbuf, strlen(sendbuf), 0) == -1)
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

    struct sockaddr_in addr, cli_addr;
    memset(&addr, 0, sizeof(addr));
    memset(&cli_addr, 0, sizeof(cli_addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    perror("bind");
    if(listen(sfd, BACKLOG) == -1)
        perror("listen");

    int cfd;
    socklen_t len = sizeof(cli_addr);

    if((cfd = accept(sfd, (struct sockaddr *)&cli_addr, &len))   == -1)
        perror("accept");
    printf("IP:%s  PORT:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    message(cfd);

    close(cfd);
    close(sfd);
    return 0;
}
