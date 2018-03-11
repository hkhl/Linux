/*************************************************************************
	> File Name: fuwuqi.c
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 16时53分05秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
/*服务器监听的本地端口*/
#define MYPORT 4000
#define MAXDATASIZE 100
/*能够同时接受多少没有accept的连接*/
#define BACKLOG 10

//多线程

int new_fd, sockfd;
pthread_t accthread, recthread;

void recmessage(void)   //接受消息
{
    while(1)
    {
        int numbytes;
        char buf[MAXDATASIZE];
        numbytes = recv(new_fd,buf,MAXDATASIZE,0);
        buf[numbytes] = '\0';
        
        //如果exit则退出
        if(strcmp(buf, "exit") == 0)
        {
            printf("Client is closed\n");
            close(new_fd);
            close(sockfd);
            exit(1);
        }
        printf("Client: %s\n",buf);
    }
}
void acceptconnect(void)   //接受客户连接
{
    struct sockaddr_in their_addr;
    int sin_size;

    
    sin_size = sizeof(struct sockaddr_in);
    if((new_fd = accept(sockfd,(struct sockaddr*)&their_addr, &sin_size))== -1)
    {
        perror("accept");
        exit(1);
    }
    printf("server:got connection from %s\n",inet_ntoa(their_addr.sin_addr));
    //创建子线程，用于接受信息
    if((pthread_create(&recthread, NULL, (void*)recmessage, NULL))!=0)
    {
        perror("create thread error!");
        exit(1);
    }
}

int main()
{

    /*在sock_fd上进行监听, new_fd接受新的连接*/
    /*自己的地址信息*/
    struct sockaddr_in my_addr;


    //创建套接字
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /*输入错误并退出*/
        perror("socket");
        exit(1);
    }

    /*初始化sockaddr_in结构体相关参数*/
    my_addr.sin_family = AF_INET; //主机字节顺序
    my_addr.sin_port = htons(MYPORT);//网络字节顺序
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero( &(my_addr.sin_zero), 8);

    /*避免出现Address alreday in use*/
    int on = 1;
    if((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,&on, sizeof(on))) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    /*绑定套接口与套接字*/
    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }


    if(listen(sockfd, BACKLOG) == -1)
    {
        //如果listen调用失败
        perror("listen");
        exit(1);
    }


    /*创建子线程，用于接收信息*/
    if((pthread_create(&accthread, NULL, (void*)acceptconnect, NULL)) != 0)
    {
        printf("create thread error!\n");
        exit(1);
    }

    while(1)
    {
        char msg[MAXDATASIZE];
        gets(msg);

        if(send(new_fd, msg, strlen(msg), 0) == -1)//发送信息
        {
            perror("send");
            exit(1);
        }
        //如果发送退出 则退出
        if(strcmp(msg, "exit") == 0)
        {
            printf("Bye Bue!\n");
            close(new_fd);
            close(sockfd);
            exit(1);
        }
    }
    return 0;
}


