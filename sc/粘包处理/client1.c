/*************************************************************************
	> File Name: kehuduan.c
	> Author: 
	> Mail: 
	> Created Time: 2017年05月01日 星期一 17时18分20秒
 ************************************************************************/
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<signal.h>
#include<unistd.h>
#define PORT 8000

struct packet
{
    int len;
    char buf[1024];
};

ssize_t readn(int fd, void *buf, size_t count)
{
    int lread = count;              //剩余未读取的长度
    int rread = 0;                  //读取的长度
    char *bufp = (char *)buf;       //用来判断读取到哪里的副本指针

    while(lread > 0)
    {
        if((rread = read(fd, bufp, lread)) < 0)
        {
            if(errno == EINTR)    //如果是信号中断
                continue;         //接着读取数据
            return -1;            //不是信号终端, 返回-1
        }
        else if(rread == 0)       //对方已经关闭
        /**/return count - lread; //读取一些,返回过去判断
                                  //函数如果没有读取完,则说明对方关闭

        bufp += rread;            //下次开始读取的地址
        lread -= rread;           //下次要读取的长度
    }
    return count;                 //读取完了返回读取的长度
}

ssize_t writen(int fd, void *buf, size_t count)
{
    int lwrite = count;              
    int rwrite = 0;               //写的长度
    char *bufp = (char *)buf;       

    while(lwrite > 0)
    {
        if((rwrite = write(fd, bufp, lwrite)) < 0)
        {
            if(errno == EINTR)    
                continue;        
            return -1;
        }
        else if(rwrite == 0)      
            continue;            //当写入为0 ,接着写

        bufp += rwrite;           
        lwrite -= rwrite;     
    }
    return count;                 
}
//从指定的socket中读取指定大小的数据,但不取出,封装后不会被信号中断
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while(1)
    {
        //MSG_PEEK 读取队列中指定大小的数据, 但不取出
        int ret = recv(sockfd, buf, len, MSG_PEEK);
        //如果被信号终端则继续读取
        if(ret == -1 && errno == EINTR)
            continue;
        return ret;
    }
}
ssize_t readline(int sockfd, void *buf, size_t maxline)
{
    int ret;
    int nread;  
    char *bufp = buf;  
    int nleft = maxline; 
    while(1)
    {
       
        ret = recv_peek(sockfd, bufp, nleft);
        if(ret < 0)    
            return ret;
        else if(ret == 0)  
            return ret;
        nread = ret;  
        
        for(int i = 0; i < nread; i++)
        {
            if(bufp[i] == '\n')
            {
                ret = readn(sockfd, bufp, i + 1);  //i+1?
                if(ret != i + 1)
                    exit(EXIT_FAILURE);
                return ret;  
            }
        }


        //当缓冲区没有\n的时候  
        if(nread > nleft)  //如果读到的数大于一行的最大数 异常处理
            exit(EXIT_FAILURE);
        nleft -= nread;    //若缓冲区没有\n, 把剩余的数据读走
        ret = readn(sockfd, bufp, nread); //将缓冲区的数据读走
        if(ret != nread)
            exit(EXIT_FAILURE);
        bufp += nread;  //bufp指针后移后, 再接着peek缓冲区数据recv_peek, 直到遇到\n

    }
    return -1;
}




void handler(int sig)
{
    if(sig == SIGUSR1)
    {
        printf("RECV:  %d\n", sig);
        exit(0);
    }
}
int main(int argc, char *argv[])
{

    signal(SIGUSR1, handler);
    int sockfd;
    struct hostent *he;
    struct sockaddr_in their_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(their_addr.sin_zero), 8);

    if(connect(sockfd, (struct sockaddr *)&their_addr,sizeof(struct sockaddr))==-1)
    {
        perror("connect");
        exit(1);
    }



    //每次循环都创建了一个进程..
    //应该是先创建子进程, 父进程while(1)负责接受 子进程while(1)负责发送

    struct packet recvbuf;
    struct packet sendbuf;
    memset(&recvbuf, 0, sizeof(recvbuf));
    memset(&sendbuf, 0, sizeof(sendbuf));
    int n;

    pid_t id = fork();
    if(id == 0)
    {
        while(1)
        {
            fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin);
            n = strlen(sendbuf.buf);
            //htonl写成htons检查了半小时
            sendbuf.len = htonl(n);        //以网络序发送过去
            writen(sockfd, &sendbuf, n+4); //发送一个结构体,带有自身的长度
            memset(&sendbuf, 0, sizeof(sendbuf));
        }
    }
    if(id > 0)
    {
        //接收消息
        while(1)
        {
            int ret = readn(sockfd, &recvbuf.len, 4);
            if(ret == -1)
            {
                perror("read");
                exit(1);
            }
            else if(ret < 4)
            {
                printf("对方已经关闭\n");
                break;
            }
            n = ntohl(recvbuf.len);
            ret = readn(sockfd, recvbuf.buf, n);
            if(ret == -1)
            {
                perror("read");
                exit(1);
            }
            else if(ret < n)
            {
                printf("对方已经关闭\n");
                break;
            }
            fputs(recvbuf.buf, stdout);
            memset(&recvbuf, 0, sizeof(recvbuf));
        }
        kill(id, SIGUSR1);
    }
close(sockfd);
    return 0;
}
