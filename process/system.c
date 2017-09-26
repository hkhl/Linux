/*************************************************************************
	> File Name: system.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月25日 星期五 19时12分35秒
 ************************************************************************/
//实现C库函数 system
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
int my_system(const char *command)
{
    pid_t id;
    int status;

    id = fork();
    if(id < 0)
        status = -1;
    else if(id == 0)
    {
        execl("/bin/sh", "sh", "-c", command, NULL);
        exit(127);
    }
    else
    {
        while(waitpid(id, &status, 0) < 0)
        {
            if(errno = EINTR)
                continue;
            status = -1;
            break;
        }
    }
    return status;
}
int main()
{
    my_system("gedit"); 
    return 0;
}
