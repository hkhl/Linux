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
#include <errno.h>           /* if(errno == ECONNRESET) */
//#include <netinet/in.h> 
//#include <signal.h>
//#include <sys/wait.h>

#include <arpa/inet.h>       /* inet_pton(AF_INET, "IP", &addr.sin_addr); */
#include <poll.h>

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
    int nready; 
    ssize_t n;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];        /* #define INET_ADDRSTRLEN 16 */
    socklen_t clilen;
    
    struct pollfd client[OPEN_MAX];
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


    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;   /* listenfd监听普通读事件 */

    for(i = 1; i < OPEN_MAX; i++)
        client[i].fd = -1;           /* 用-1初始化client[].fd里剩下的元素 */
    maxi = 0;

    while(1)
    {
        if( (nready = poll(client, maxi+1, -1)) < 0)   /* -1阻塞 */
        ERR_EXIT("poll.\n");

        /* 这里不能用 ==,renents是16位,某个事件的触发占用其中一位,有可能是多个事件的触发 */
        if(client[0].revents & POLLRDNORM)  /* 有客户端连接请求 */
        { 
            /* 编译的时候计算好了, 并非每次连接都计算一次 */     
            clilen = sizeof(cliaddr);

            if( (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen )) < 0 )
            ERR_EXIT("accept");
            printf("client: IP:%s  PORT:%d\n", inet_ntop(AF_INET,&cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));

            for(i = 1; i < OPEN_MAX; i++)
            {
                if(client[i].fd < 0)
                {
                    client[i].fd = connfd;
                    break;
                }
            }
            if(i == OPEN_MAX)
                ERR_EXIT("too many clients");
            client[i].events = POLLRDNORM;   /* 设置刚刚返回的connfd,监控读事件 */
            if(i > maxi)
                maxi = i;
            if(--nready == 0)        /* 只有一个处理连接的请求过来, 则处理完回到上面select阻塞监听集合中的fd*/
                continue;  
        }
        for(i = 1; i <= maxi; i++)
        {
            if((sockfd = client[i].fd) < 0)
                continue;
            if(client[i].revents & (POLLRDNORM | POLLERR))
            {
                if( (n = read(sockfd, buf ,MAXLINE)) < 0)
                {
                    if(errno == ECONNRESET)  /* 当收到RST标志时 */
                    { /* connection reset by client */
                        printf("client[%d] aborted connection.\n", i);
                        close(sockfd);
                        client[i].fd = -1;
                    }
                    else
                    {
                        ERR_EXIT("read error.\n");
                    }
                }
                else if(n == 0)
                { /* connection closed by client */ 
                    printf("client[%d] closed connection.\n", i);
                    close(sockfd);
                    client[i].fd = -1;
                }
                else
                {
                    for(j = 0; j < n; j++)
                        buf[j] = toupper(buf[j]);
                    write(sockfd, buf, n);
                }
                if(--nready == 0)
                    break;
            }
        }
    }
    return 0;
}
