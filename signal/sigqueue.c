/*************************************************************************
	> File Name: signal_queue.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月02日 星期六 10时47分42秒
 ************************************************************************/
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

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
 int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask;
    int        sa_flags;                       //接收带有消息的 与这个参数有关.
    void     (*sa_restorer)(void);
};
*/
void sa_sig(int num, siginfo_t *info, void *p)
{
    printf("RECV : %d\n", num);
    int n;
    n = info->si_value.sival_int;
    printf("RECV:  %d     %d\n", n, info->si_int);

}
    //sigval_t si_value;     /* Signal value */
    //int      si_int;       /* POSIX.1b signal */

int main()
{
    //注册一个信号
    struct sigaction act;

    //act.sa_handler = handler;
    //sigaction(SIGINT, &act, NULL);


    //复杂版本, 可以接收额外信息
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sa_sig;  //相当于回调函数
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);

    if(sigaction(SIGINT, &act, NULL))
    perror("sigaction");

    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
    }
    else if(id == 0)
    {
        //给父进程发送消息
        /*
        union sigval {
        int   sival_int;
        void *sival_ptr;
    };
        */
        union sigval mysigval;
        mysigval.sival_int = 222;
        sigqueue(getppid(), SIGINT, mysigval);
        exit(0);
    }


    while(1)
    {
        pause();
    }
    return 0;
}
