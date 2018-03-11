/*************************************************************************
	> File Name: kehuduan.c
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 17时18分20秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>

#define PORT 4000
#define MAXDATASIZE 100

//多线程

int sockfd;
pthread_t recthread;
void recmessage()   //接受消息
{
    while(1)
    {
        int numbytes;
        char buf[MAXDATASIZE];

        numbytes = recv(sockfd,buf,MAXDATASIZE,0);
        buf[numbytes] = '\0';
        //如果exit则退出
        if(strcmp(buf, "exit") == 0)
        {
            printf("Server is closed\n");
            close(sockfd);
            exit(1);
        }
        printf("Server: %s\n",buf);
    }
}

int main(int argc, char *argv[])
{
    struct hostent *he;
    struct sockaddr_in their_addr;

    //客户端输入方式：
    //./client 127.0.0.1 若没有输入IP地址 会报错
    if(argc != 2)
    {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    //获取主机IP地址
    if((he = gethostbyname(argv[1]))==NULL)
    {
        herror("gethostbyname");
        exit(1);
    }

    //创建套接字
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        perror("socket");
        exit(1);
    }

    /*初始化sockaddr_in结构体相关参数*/
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr*)he->h_addr);
    bzero(&(their_addr.sin_zero), 8);

    if(connect(sockfd, (struct sockaddr *)&their_addr,sizeof(struct sockaddr))==-1)
    {
        perror("connect");
        exit(1);
    }

    //创建子线程，接收信息
    if((pthread_create(&recthread, NULL, (void*)recmessage, NULL)) != 0)
    {
        //perror("connect");
        printf("error");
        exit(1);
    }
    
    while(1)
    {
        char msg[MAXDATASIZE];
        gets(msg);
        
        if(send(sockfd, msg, strlen(msg), 0) == - 1)
        {
            perror("send");
            exit(1);
        }
        if(strcmp(msg, "exit") == 0)
        {
            printf("Bye Bye\n");
            close(sockfd);
            exit(1);
        }
    }
return 0;
}
