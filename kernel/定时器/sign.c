/*************************************************************************
	> File Name: sign.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月20日 星期三 14时52分59秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

void hello();
int main()
{
    int pid;
    while( (pid=fork()) == -1);  //创建子进程
    if(pid > 0)
    {
        printf("Nice to meet you, I'm parent process !\n");
        kill(pid, SIGUSR1);
        wait(NULL);
        sleep(5);
        printf("parent process is killed !\n");
        _exit(0);
    }
    else
    {
        signal(SIGUSR1, hello);
        sleep(3);
        printf("Child process is keiiled by parent !\n");
        _exit(0);
    }
    return 0;
}
void hello()
{
    printf("Hello, welcome to child process !\n");
    printf("Goodbye, quit...\n");
}
