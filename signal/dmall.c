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
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void handler(int sig)
{
    if(sig == SIGUSR1)
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
    else if(sig == SIGINT || sig == SIGRTMIN)
    {
        if(sig == SIGINT)
        printf("接受到阻塞信号%d\n", sig);
        if(sig == SIGRTMIN)
        printf("接受到非阻塞信号%d\n", sig);
    }
    else
    {
        printf("其他信号\n");
    }
}
int main()
{
    //注册非实时信号
    if(signal(SIGINT, handler) == SIG_ERR)
    perror("SIGINT");
    //注册用户自定义信号
    if(signal(SIGUSR1, handler) == SIG_ERR)
    perror("SIGUSR1");
    //注册实时信号
    if(signal(SIGRTMIN, handler) == SIG_ERR)
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
        for(int i = 0; i < 3; i++)
        {
            v.sival_int = 301 + i;
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
