/*************************************************************************
	> File Name: kill.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月02日 星期六 13时09分58秒
 ************************************************************************/
//kill函数

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
void handler(int num)
{
    printf("信号SIGSUR1\n");
}
int main()
{
    pid_t id = fork();
    
    if(signal(SIGUSR1, handler) == SIG_ERR)
    {
        perror("signal");
        return 0;
    }
    //子进程向父进程发送信号
    if(id == 0)
    {
        //pid_t pid = getppid();
        //kill(pid, SIGUSR1);             //向父进程发送信号
        //kill(0, SIGUSR1);               //向进程组发送信号
        exit(0);
    }

    int n = 3;
    do
    {
        printf("sleeping\n");
        n = sleep(n);
        printf("sleeped\n");
    }while(n > 0);
    printf("end\n");
    return 0;}
