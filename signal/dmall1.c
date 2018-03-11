/*************************************************************************
	> File Name: dmall.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月03日 星期日 16时06分43秒
 ************************************************************************/
 
//创建实时信号与非实时信号加入到进程阻塞当中
//注册用户自定义信号
//子进程带数据发送个三次信号 ,, 实时和非实时
//子进程发送SIGSUR1信号解除阻塞
//观察实时信号与非实时信号的表现与区别.
//
//基础上增加接受发送的数据
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void sighandler(int signum, siginfo_t *info, void *p)
{
    if(signum == SIGUSR1)
    {
        //解除阻塞信号函数
        sigset_t bset;
        sigemptyset(&bset);
        sigaddset(&bset, SIGINT);
        sigaddset(&bset, SIGRTMIN);
        //int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
        sigprocmask(SIG_UNBLOCK, &bset, NULL);
        printf("解除阻塞\n");
    }
    else if(signum == SIGINT || signum == SIGRTMIN)
    {
        if(signum == SIGINT)
        printf("接受到阻塞信号%d  内容 %d\n ", signum, info->si_int);
        if(signum == SIGRTMIN)
        printf("接受到非阻塞信号%d  内容 %d\n", signum, info->si_value.sival_int);
    }
    else
    {
        printf("其他信号\n");
    }
}
int main()
{
    //注册非实时信号
/*
       int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
    struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*/
    struct sigaction act;
    act.sa_sigaction = sighandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    if(sigaction(SIGINT, &act, NULL)) 
    perror("SIGINT");
    //注册用户自定义信号
    if(sigaction(SIGUSR1, &act, NULL))
    perror("SIGUSR1");
    //注册实时信号
    if(sigaction(SIGRTMIN, &act, NULL))
    perror("SIGRTMIN");

    sigset_t bset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGINT);
    sigaddset(&bset, SIGRTMIN);
    //int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
    sigprocmask(SIG_BLOCK, &bset, NULL);
    
    /*
    int sigqueue(pid_t pid, int sig, const mZnion sigval value);
    union sigval {
    int   sival_int;
    void *sival_ptr;
                };
    * */

    pid_t id = fork();
    if(id == -1)
    {
        perror("fork");
    }
    else if(id == 0)
    {
        int ret;
        union sigval v;
        for(int i = 0; i < 3; i++)
        {
            v.sival_int = 201 + i;
            ret = sigqueue(getppid(), SIGINT, v);
            if(ret != 0)
            {
                perror("sigqueue SIGINT");
            }
            else
            {
                printf("发送不可靠信号OK\n");
            }

        }
        for(int i = 0; i < 9*1024; i++)
        {
            v.sival_int =  i;
            ret = sigqueue(getppid(), SIGRTMIN, v);
            if(ret != 0)
            {
                perror("sigqueue SIGINT");
            }
            else
            {
                printf("发送可靠信号OK\n");
            }
            kill(getppid(), SIGUSR1);
        }
    }

    while(1);

    return 0;
}
