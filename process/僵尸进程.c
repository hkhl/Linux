/*************************************************************************
	> File Name: 僵尸进程.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月25日 星期五 17时49分22秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    printf("fork\n");
    pid_t id  = fork();
    if(id > 0)
    {
        sleep(10);
    }
    if(id == 0)
    {
        exit(0);
    }
    wait(NULL);

    return 0;
}
