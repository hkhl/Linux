/*************************************************************************
	> File Name: mysignal.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月03日 星期日 13时24分22秒
 ************************************************************************/
//用sigaction实现signal函数

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>

void handler(int sig)
{
    printf("The sig is %d\n", sig);
}
/*       int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact);
*/
/*
*
           struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*
* */

__sighandler_t my_signal(int signum, __sighandler_t handler)
{
    struct sigaction act;
    struct sigaction oldact;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    if(sigaction(signum, &act, &oldact) < 0)
    {
        return SIG_ERR;
    }

    return oldact.sa_handler; 
}

int main()
{
    __sighandler_t old = my_signal(SIGINT, handler);
    /*if(my_signal(SIGINT, handler) == SIG_ERR)
    perror("my_signal");
    */
    char ch;
    while((ch = getchar()) != 'x')
    {
        pause();
        printf("123\n");
    }
    //my_signal(SIGINT, old);

    return 0;
}
