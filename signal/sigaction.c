/*************************************************************************
	> File Name: signal_action.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月02日 星期六 10时47分42秒
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>

void* handler(int num)
{
    printf("recv: %d\n", num);
    if(num == SIGINT)
    {
        printf("hahaha\n");
    }
    return NULL;
}


/*
 *
                             int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);

DESCRIPTION
       The sigaction() system call is used to change the action taken by a process on receipt of a specific signal.  (See signal(7) for an overview of signals.)

       signum specifies the signal and can be any valid signal except SIGKILL and SIGSTOP.

       If act is non-NULL, the new action for signal signum is installed from act.  If oldact is non-NULL, the previous action is saved in oldact.

       The sigaction structure is defined as something like:

           struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*/

void sa_sig(int num, siginfo_t *info, void *p)
{
    printf("RECV : %d\n", num);
}

int main()
{
    //注册一个信号
    struct sigaction act;
    
    //act.sa_handler = handler;
    //sigaction(SIGINT, &act, NULL);
    
    act.sa_sigaction = sa_sig;  //相当于回调函数
    sigaction(SIGINT, &act, NULL);
    
    char ch;
    printf("输入x退出\n");
    while((ch = getchar()) != 'x');
    {
        pause();      //pause 函数 挂起 可被信号中断
    }
    //恢复默认信号
    signal(SIGINT, SIG_DFL);
    //signal(SIGINT, old);

    while(1);
    return 0;
}
