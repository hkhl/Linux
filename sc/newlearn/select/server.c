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
#include <netinet/in.h> 
#include <ctype.h>
//#include <sys/types.h>  
//#include <sys/socket.h>
//#include <errno.h>

//#include <signal.h>
//#include <sys/wait.h>

#include <arpa/inet.h>       /*inet_pton(AF_INET, "IP", &addr.sin_addr);*/

#define MAXLINE 80
#define SERV_PORT 8000

#define ERR_EXIT(x) \
    {\
        perror(x); \
        exit(EXIT_FAILURE);}

int main()
{
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];   /*FD_SERSIZE 默认为 1024 */
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];        /* #define INET_ADDRSTRLEN 16 */
    socklen_t cliaddr_len;
    struct sockaddr_in cliaddr, servaddr;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    ERR_EXIT("socket");


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    ERR_EXIT("bind");

    if(listen(listenfd, 20) < 0)
    ERR_EXIT("listen");


    maxfd = listenfd;
    maxi = -1;

    for(i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);


    while(1)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if(nready < 0)
        ERR_EXIT("select");
        
        if(FD_ISSET(listenfd, &rset))    /*new client connection*/
        {
            cliaddr_len = sizeof(cliaddr);
            if( (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len )) < 0 )
            ERR_EXIT("accept");
            printf("client: IP:%s  PORT:%d\n", inet_ntop(AF_INET,&cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
                
            for(i = 0; i < FD_SETSIZE; i++)
            {
                if(client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            }
            if(i == FD_SETSIZE)
            {
                fputs("too many clients\n", stderr);
                exit(1);
            }

            FD_SET(connfd, &allset);  /* 将新的fd添加到信号集里 */
            if(connfd > maxfd)
                maxfd = connfd;
            if(i > maxi)
                maxi = i;

            if(--nready == 0)        /* 只有一个处理连接的请求过来, 则处理完回到上面select阻塞监听集合中的fd*/
                continue;  
        }

        for(i = 0; i <= maxi; i++)
        {
            if( (sockfd = client[i]) < 0)
                continue;
            if(FD_ISSET(sockfd, &rset))
            {
                if( (n = read(sockfd, buf, MAXLINE)) == 0)
                {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                else
                {
                    for(int j = 0; j < n; j++)
                        buf[j] = toupper(buf[j]);
                    write(sockfd, buf, n);
                    memset(buf, 0, sizeof(buf));
                }
                if(--nready == 0)
                    break;
            }
        }
    }
    close(listenfd);
    return 0;
}
