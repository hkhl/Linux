/*************************************************************************
	> File Name: avoid.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月25日 星期五 19时12分35秒
 ************************************************************************/
//避免僵尸进程
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
int main()
{
    signal(SIGCHLD, SIG_IGN); 
    pid_t id  = fork();
    if(id > 0)
    {
        sleep(20);
    }
    if(id == 0)
    {
        exit(0);
    }


    return 0;
}
