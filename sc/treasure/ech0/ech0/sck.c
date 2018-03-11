/*************************************************************************
	> File Name: sck.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月12日 星期二 21时01分26秒
 ************************************************************************/
#include "sck.h"

void handler(int sig)
{
    printf("RECV SIG: %d\n", sig);
    exit(0);
}
void sigchld_handler(int sig)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

/* read函数的调用方法
int ret;
ret = read_timeout(fd, 5);
if (ret == 0)
{
	read(fd, ...);
}
else if (ret == -1 && errno == ETIMEDOUT)
{
	timeout....
}
else
{
	ERR_EXIT("read_timeout");
}
*/

/**
 * read_timeout - 读超时检测函数，不含读操作
 * @fd: 文件描述符
 * @wait_seconds: 等待超时秒数，如果为0表示不检测超时
 * 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT
 */
int read_timeout(int fd, unsigned int wait_seconds)
{
    int ret = 0;
    if(wait_seconds > 0)
    {
        fd_set read_fdset;
        struct timeval timeout;

        FD_ZERO(&read_fdset);
        FD_SET(fd, &read_fdset);

        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;

        //select 返回值三态
        //1 若timeout时间到(超时),没有检测到读时间 ret返回0
        //2 若ret返回<0 && errno == EINTR select过程中被信号中断(可中断睡眠)
        //  仅仅是<0 errno!=EINTR 则是出错
        //3 若ret返回>0 表示read事件发生, 返回的是事件发生的个数
        
        do
        {
            ret = select(fd+1, &read_fdset, NULL, NULL, &timeout);
        }while(ret < 0 && errno == EINTR);  //不能被信号中断

        if(ret == 0)     //超时 
        {
            ret = -1;    //返回-1 且设置错误码ETIMEDOUT
            errno = ETIMEDOUT;
        }
        else if(ret == 1)  //当事件在时间内发生时
        {
            ret = 0;   //返回0
        }
    }
    return ret;
}

//同read_timeout函数
//成功(未超时)返回>=0  超时失败返回-1 错误码ETIMEDOUT 
int write_timeout(int fd, unsigned int wait_seconds)
{
    int ret = 0;
    if(wait_seconds > 0)
    {
        fd_set write_fdset;
        struct timeval timeout;

        FD_ZERO(&write_fdset);
        FD_SET(fd, &write_fdset);

        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;

        do
        {
            ret = select(fd+1, &write_fdset, NULL, NULL, &timeout);
        }while(ret < 0 && errno == EINTR);  //不能被信号中断

        if(ret == 0)     //超时 
        {
            ret = -1;    //返回-1 且设置错误码ETIMEDOUT
            errno = ETIMEDOUT;
        }
        else if(ret == 1)  //当事件在时间内发生时
        {
            ret = 0;   //返回0
        }
    }
    return ret;
}
//思想：tcpip是流协议，不能保证1次读操作，能全部把报文读走，所以要循环读指定长度的数据。
//按照count大小读数据，
//若读取的长度ssize_t<count 说明读到了一个结束符，对方已关闭。
//
//@ssize_t：返回读的长度 若ssize_t<count 读失败失败
//@buf：接受数据内存首地址
//@count：接受数据长度
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
            return count - lread; //读取一些,返回过去判断
                                  //函数如果没有读取完,则说明对方关闭

        bufp += rread;            //下次开始读取的地址
        lread -= rread;           //下次要读取的长度
    }
    return count;                 //读取完了返回读取的长度
}

ssize_t writen(int fd, void *buf, size_t count)
{
    int lwrite = count;
    int rwrite = 0;
    char *bufp = (char *)buf;

    while(lwrite > 0)
    {
        if((rwrite = write(fd, bufp, lwrite))< 0)
        {
            if(errno == EINTR)
                continue;
            return -1;
        }
        else if(rwrite == 0)
            continue;
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

//maxline 一行最大数
//先提起peek以下缓冲区,如果由数据从缓冲区读数据
//1. 缓冲区数据中带有\n
//2. 缓冲区数据中不带有\n
//   当不带有\n的时候,读取数据包一直到读到\n为止
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


void cs_send(int fd)
{
    struct Packet sendbuf;
    memset(&sendbuf, 0, sizeof(sendbuf));
    int n;
    while(fgets(sendbuf.buf, sizeof(sendbuf), stdin) > 0)
    {
        n = strlen(sendbuf.buf);
        sendbuf.len = htonl(n);
        writen(fd, &sendbuf, n+4);
        memset(&sendbuf, 0, sizeof(sendbuf));
    }
}
int cs_recv(int fd)
{
    struct Packet recvbuf;
    memset(&recvbuf, 0, sizeof(recvbuf));
    int n;
    while(1)
    {
        int ret = readn(fd, &recvbuf.len, 4);
        if(ret == -1)
            ERR_EXIT("readn1");
        else if(ret < 4)
        {
            printf("对方已经关闭\n");
            break;
        }
        n = ntohl(recvbuf.len);
        ret = readn(fd, recvbuf.buf, n);
        if(ret == -1)
            ERR_EXIT("readn2");
        else if(ret < n)
        {
            printf("对方已经关闭\n");
            break;
        }
        fputs(recvbuf.buf, stdout);
        memset(&recvbuf, 0, sizeof(recvbuf));
    }
    return 0;
}



