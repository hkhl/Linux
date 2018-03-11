/*************************************************************************
	> File Name: sck.h
	> Author: 
	> Mail: 
	> Created Time: 2017年09月12日 星期二 21时32分37秒
 ************************************************************************/

#ifndef _SCK_H
#define _SCK_H
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
#define PORT 8000
#define ERR_EXIT(x) \
    do \
    { \
        perror(x); \
        exit(1); \
    }while(0)      //分号的有无???


//自定义封包的结构体
struct Packet 
{
    int len;         //自定义的包头, 是数据包的大小
    char buf[1024];  //包存放的数据
};


void handler(int sig);
void sigchld_handler(int sig);
ssize_t readn(int fd, void *buf, size_t count);
ssize_t writen(int fd, void *buf, size_t count);
void cs_send(int fd);
int cs_recv(int fd);


#endif
