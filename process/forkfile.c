/*************************************************************************
	> File Name: forkfile.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月25日 星期五 19时45分34秒
 ************************************************************************/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
    signal(SIGCHLD, SIG_IGN);
    int fd = open("./1.txt", O_RDWR);
    if(fd == -1)
    {
        perror("open");
    }

    pid_t id  = fork();
    if(id < 0)
    {
        perror("error");
    }
    else if(id > 0)
    {
        write(fd, "hello", 5);
        close(fd);
    }
    else if(id == 0)
    {
        write(fd, "wang", 4);
        close(fd);
        exit(0);
    }

    return 0;
}
