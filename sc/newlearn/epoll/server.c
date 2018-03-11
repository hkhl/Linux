/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月29日 星期日 23时23分30秒
 ************************************************************************/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <ctype.h>           /* toupper(); */
//#include <sys/types.h>  
//#include <sys/socket.h>
//#include <errno.h>        
//#include <netinet/in.h> 
//#include <signal.h>
//#include <sys/wait.h>

#include <arpa/inet.h>       /* inet_pton(AF_INET, "IP", &addr.sin_addr); */
#include <sys/epoll.h>

#define MAXLINE 80
#define SERV_PORT 8000
#define OPEN_MAX 1024

#define ERR_EXIT(x) \
    do{\
        perror(x); \
        exit(EXIT_FAILURE);\
    }while(0)

int main()
{
    int i, j, on, maxi, listenfd, connfd, sockfd;
    int nready , efd, res; 
    ssize_t n;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];        /* #define INET_ADDRSTRLEN 16 */

    int client[OPEN_MAX];
    socklen_t clilen;
    
    /***********************************/
    struct epoll_event tep, ep[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;
    
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    ERR_EXIT("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    on = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    ERR_EXIT("setsockopt");

    if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    ERR_EXIT("bind");

    if(listen(listenfd, 20) < 0)
    ERR_EXIT("listen");

    for(i = 0; i < OPEN_MAX; i++)
        client[i] = -1;           
    maxi = -1;

    if( (efd = epoll_create(OPEN_MAX)) < 0)
    ERR_EXIT("epoll_create");

    tep.events = EPOLLIN; tep.data.fd = listenfd;
    if( (res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep)) < 0)
    ERR_EXIT("epoll_ctl");

    while(1)
    {
        if( (nready = epoll_wait(efd, ep, OPEN_MAX, -1)) < 0)   /* -1阻塞 */
        ERR_EXIT("epoll_wait\n");

        for(i = 0; i < nready; i++)
        {
            if(!(ep[i].events & EPOLLIN))    /* 如果不是这个EPOLLIN响应 */
                continue;
            if(ep[i].data.fd == listenfd)
            {
                clilen = sizeof(cliaddr);

                if( (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen )) < 0 )
                ERR_EXIT("accept");
                printf("client: IP:%s  PORT:%d\n", inet_ntop(AF_INET,&cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
                for(j = 0; j < OPEN_MAX; j++)
                {
                    if(client[j] < 0)
                    {
                        client[j] = connfd;
                        break;
                    }
                }
                if(j == OPEN_MAX)
                ERR_EXIT("too many clients");
                if(j > maxi)
                maxi = j;
                /* 设置刚刚返回的connfd,监控读事件 */
                tep.events = EPOLLIN; tep.data.fd = connfd;
                if( (res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep)) < 0)
                ERR_EXIT("epoll_ctl");
            }
            else
            {
                sockfd = ep[i].data.fd;
                if( (n = read(sockfd, buf ,MAXLINE)) == 0)
                {
                    for(j = 0; j <= maxi; j++)
                    {
                        if(client[j] == sockfd)
                        {
                            client[i] = -1;
                            break;
                        }
                    }
                    /************************************************************null??*/
                    if( (res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL)) < 0)
                    ERR_EXIT("epoll_ctl");
                    close(sockfd);
                    printf("client[%d] closed connection\n", j);
                }
                else
                {
                    for(j = 0; j < n; j++)
                        buf[j] = toupper(buf[j]);
                    write(sockfd, buf, n);
                }
            }

        }
    }
    close(listenfd);
    close(efd);
    return 0;
}
