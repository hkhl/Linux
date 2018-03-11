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
