/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月29日 星期日 22时53分55秒
 ************************************************************************/
//完成epoll对应的客户端模型

#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
//#include <sys/types.h>  
//#include <sys/socket.h>
//#include <errno.h>
//#include <signal.h>
//#include <sys/wait.h>

//#include <netinet/in.h> 
#include <arpa/inet.h>       /*inet_pton(AF_INET, "IP", &addr.sin_addr);*/
#include <sys/epoll.h>
#define MAXLINE 80
#define OPEN_MAX 1024
#define SERV_PORT 8000

#define ERR_EXIT(x) \
    do{\
        perror(x); \
        exit(EXIT_FAILURE);\
    }while(0)


int main()
{
    int i, j, maxi, sockfd, n;
    int nready, efd, res;
    int client[OPEN_MAX];
    struct epoll_event tep, ep[OPEN_MAX];

    struct sockaddr_in servaddr;
    char buf[MAXLINE];

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0))  < 0 )
    ERR_EXIT("socket");
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    ERR_EXIT("connect");

    for(i = 0; i < OPEN_MAX; i++)
        client[i] = -1;
    maxi = -1;

    if( (efd = epoll_create(OPEN_MAX)) < 0)
    ERR_EXIT("epoll_create");


    tep.events = EPOLLIN; tep.data.fd = sockfd;
    if( (res = epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &tep)) < 0)
    ERR_EXIT("epoll_crl");

    int fd_stdin = fileno(stdin);
    while(1)
    {
        
    }


    while(fgets(buf, MAXLINE, stdin) != NULL)
    {
        write(sockfd, buf, MAXLINE);
        memset(buf, 0, sizeof(buf));
        
        n = read(sockfd, buf, MAXLINE);
        if(n == 0)
            printf("the server has been closed.\n");
        else
            printf("%s\n", buf);
    }

    close(sockfd);

    return 0;
}
