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
#include<signal.h>
#define MYPORT 8000
#define BACKLOG 10

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

void handler(int sig)               //信号处理函数
{
    if(sig == SIGUSR1)
    {
        printf("RECV:  %d\n", sig);
        exit(0);
    }
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

//maxline 一行最大数
//先提起peek以下缓冲区,如果由数据从缓冲区读数据
//1. 缓冲区数据中带有\n
//2. 缓冲区数据中不带有\n
//   当不带有\n的时候,读取数据包一直到读到\n为止
ssize_t readline(int sockfd, void *buf, size_t maxline)
{
    int ret;
    int nread;  //读取的数据个数
    char *bufp = buf;  //读取数据存放的数组,在外分配内存
    int nleft = maxline; //剩余未读取的数据包长度, 也是封包的最大值
    while(1)
    {
        //偷看一下缓冲区有没有数据, 并不移除内核缓冲区的数据
        ret = recv_peek(sockfd, bufp, nleft);
        if(ret < 0)    //读取失败
            return ret;
        else if(ret == 0)  //对方已经关闭
            return ret;
        nread = ret;   //读取了ret个数据
        
        for(int i = 0; i < nread; i++)
        {
            if(bufp[i] == '\n')//若读的缓冲区中有\n
            {
                ret = readn(sockfd, bufp, i + 1); //读走数据   i+1?
                if(ret != i + 1)
                    exit(EXIT_FAILURE);
                return ret;   //有\n就返回, 并返回读走的数据
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



int main()
{
    signal(SIGUSR1, handler);
    int sockfd, new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    int len;

    if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero( &(my_addr.sin_zero), 8);

    int on = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    perror("setsockopt");

    if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    while(1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if((new_fd = accept(sockfd,(struct sockaddr*)&their_addr, &sin_size))== -1)
        {
            perror("accept");
            continue;
        }
        printf("server: got connection fron %s\n", inet_ntoa(their_addr.sin_addr));
            
        
        struct packet recvbuf;
        struct packet sendbuf;
        memset(&recvbuf, 0, sizeof(recvbuf));
        memset(&sendbuf, 0, sizeof(sendbuf));
        int n;
        pid_t id = fork();
        if(id == 0)
        { 
            //发送数据
            while(1)
            {
                fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin);
                n = strlen(sendbuf.buf);
                sendbuf.len = htonl(n);        //以网络序发送过去
                writen(new_fd, &sendbuf, n+4); //发送一个结构体,带有自身的长度
                memset(&sendbuf, 0, sizeof(sendbuf));
            }
        }
        if(id > 0)
        {
            while(1)
            {
                //接收两次 先读取报头(长度)  再读取报文(buf内容)
                int ret = readn(new_fd, &recvbuf.len, 4);//读取结构体中报文的长度
                if(ret == -1)
                {
                    perror("read1");
                    exit(1);
                }
                else if(ret < 4)           //没有读取完,则是对方关闭
                {
                    printf("对方已经关闭\n");
                    break;
                }
               
                n = ntohl(recvbuf.len);   //将读取到的长度转换为主机字节序
                ret = readn(new_fd, recvbuf.buf, n);
                if(ret == -1)
                {
                    perror("read2");
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
            close(new_fd);
            //当客户端退出的时候, 这边父进程关闭,并给子进程发送信号关闭子进程
            kill(id, SIGUSR1);
            sleep(1);
            break;
        } 
    }
close(sockfd);
return 0;
}
