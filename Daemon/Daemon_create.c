/*************************************************************************
	> File Name: Daemon.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月26日 星期六 23时45分08秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

//创建守护进程

int main()
{
    pid_t id = fork();
    if(id == -1)
    {
        perror("fork");
        exit(1);
    }
    if(id > 0)
    {
        exit(EXIT_SUCCESS);
    }
    int n = setsid();
    if(n == -1)
    {
        perror("sersid");
        exit(1);
    }

    printf("%d\n", getpid()); 
    chdir("/");
    for(int i = 0; i < 3; i++)
    {
        close(i);
        open("/dev/null", O_RDWR);
    }
    dup(0);
    dup(0);
    umask(0); 

    return 0;
}




