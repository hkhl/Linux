/*************************************************************************
	> File Name: srv.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月12日 星期二 20时04分38秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "sck.h"
#define PORT 8000
#define ERR_EXIT(x) \
    do \
    { \
        perror(x); \
        exit(1); \
    }while(0)      //分号的有无???

int main()
{
    signal(SIGUSR1, handler);
    signal(SIGCHLD, sigchld_handler);  //处理僵尸进程
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket");
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 

    //地址复用
    int on = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");
    
    //bind
    if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr) ) < 0)
        ERR_EXIT("bind");

    //listen
    if(listen(sockfd,SOMAXCONN) < 0)
        ERR_EXIT("liten");
    
    //accept
    int conn;
    struct sockaddr_in peeraddr;
    memset(&peeraddr, 0, sizeof(peeraddr));
    socklen_t len = sizeof(peeraddr);
    
    while(1)
    {
        if((conn = accept(sockfd, (struct sockaddr *)&peeraddr, &len )) < 0)
            ERR_EXIT("accept");
        printf("id = %s, port = %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
        
        pid_t id = fork();
        if(id < 0)
            ERR_EXIT("fork"); 
        if(id == 0)
        {
            cs_send(conn);
        }
        if(id > 0)
        {
            int ret = cs_recv(conn);  
            if(ret == 0)
            {
                kill(id, SIGUSR1);
            }
        }
    }
    return 0;
}
