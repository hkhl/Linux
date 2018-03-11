/*************************************************************************
	> File Name: signal_SIGINT.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月02日 星期六 10时47分42秒
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
//signal simple demons
void* handler(int num)
{
    printf("recv: %d\n", num);
    if(num == SIGINT)
    {
        printf("hahaha\n");
    }
    return NULL;
}
int main()
{
    //注册一个信号
    //__sighandler_t old = signal(SIGINT, handler); 
    /*old 相当于SIG_DFL 默认信号 执行原来的行为*/

    if(signal(SIGINT, handler) == SIG_ERR)
    {
        perror("signal");
        exit(0);
    }

    char ch;
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
