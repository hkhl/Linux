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
void sa_sig(int num, siginfo_t *info, void *p)
{
    printf("RECV : %d\n", num);
    int n;
    n = info->si_value.sival_int;
    printf("RECV:  %d     %d\n", n, info->si_int);
}

int main()
{
    struct sigaction act;

    //复杂版本, 可以接收额外信息
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sa_sig;  //相当于回调函数
    sigemptyset(&act.sa_mask);
    if(sigaction(SIGINT, &act, NULL))
    perror("sigaction");

    while(1)
    {
        printf("wait messages:");
        pause();
        printf("haha \n");
    }
    return 0;
}
