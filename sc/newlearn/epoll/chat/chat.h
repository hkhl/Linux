/*************************************************************************
	> File Name: chat.h
	> Author: 
	> Mail: 
	> Created Time: 2017年11月08日 星期三 19时44分11秒
 ************************************************************************/

#ifndef _CHAT_H
#define _CHAT_H
#endif
#include<stdio.h> 
#include<stdlib.h>
#include<string.h> 
#include<errno.h>        
#include<signal.h>
#include<unistd.h> 
#include<sys/types.h>  
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/epoll.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<netinet/in.h> 
#define MAXLINE 80
#define SERV_PORT 8000
#define OPEN_MAX 1024

#define ERR_EXIT(x) \
    do{\
        perror(x); \
         exit(EXIT_FAILURE);\
    }while(0)

struct Protocol
{
    int id;             //协议号
    int fd;             //fd
    char src[20];       //发送方
    char dest[20];      //接收方
    char data[1024];    //数据内容
};





