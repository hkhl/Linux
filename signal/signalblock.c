/*************************************************************************
	> File Name: signalblock.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月02日 星期六 19时27分59秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
void handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("recv a sig=%d\n", sig);
        printf("\n\n\n");
        //fflush(stdout);
    }
    else if (sig == SIGQUIT)
    {
        sigset_t uset;
        sigemptyset(&uset);
        sigaddset(&uset, SIGINT);
        //ctr + \ 用来解除  SIGINT 信号
        //解除阻塞
        sigprocmask(SIG_UNBLOCK, &uset, NULL);
        printf("被阻塞的信号到了@!");
        signal(SIGINT, SIG_DFL) ;  
    }
}
void printfsigpending(sigset_t *set)
{
    int i = 0;
    for(int i = 1; i < NSIG; i++)
    {
        if(sigismember(set, i))    //看i是否在集合中
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
    putchar(10);
}

int main()
{
    sigset_t pset;               //未决状态字的集合  pending
    sigset_t bset;               //阻塞状态字的集合  block

    sigemptyset(&bset);          //清空信号集(64位)
    sigaddset(&bset, SIGINT);    //将sigint信号放入信号集, 
                                 //即将第二位置为1
    if(signal(SIGINT, handler) == SIG_ERR)
    {
        perror("sigint");
    }
    if(signal(SIGQUIT, handler) == SIG_ERR)
    {
        perror("sigquit");
    }

    //信号被阻塞  即使按了CTRL+C 也没用
    sigprocmask(SIG_BLOCK, &bset, NULL);

    while(1)
    {
        //获取未决 字的状态
        sigpending(&pset);
        printfsigpending(&pset);
        sleep(1);
    }
    return 0;
}
