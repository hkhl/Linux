/*************************************************************************
	> File Name: Daemon_achieveAPI.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月26日 星期二 10时46分38秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include<unistd.h>

#define ERR_EXIT(m) \
do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
}\
while (0);\

int create_daemon(int nochdir, int noclose);
int main()
{
    time_t t;
    int fd;
    if(create_daemon(0,0) == -1)
        ERR_EXIT("daemon error");
    while(1)
    {
        fd = open("daemon.log",O_WRONLY|O_CREAT|O_APPEND,0644);
        if(fd == -1)
            ERR_EXIT("open error");
        t = time(0);
        char *buf = asctime(localtime(&t));
        write(fd,buf,strlen(buf));
        close(fd);
        sleep(5);
    }
    return 0;
}
int create_daemon(int nochdir, int noclose)
{
    pid_t id = fork();
    if(id == -1)
    {
        perror("fork");
        exit(1);
    }
    else if(id > 0)
    {
        exit(EXIT_SUCCESS);
    }

    int n  = setsid();
    if(n == -1)
    {
        perror("sersid");
        exit(1);
    }

    if(nochdir == 0)
    {
        chdir("/");
    }
    if(noclose == 0)
    {
        for(int i = 0; i < 3; i++)
        {
            close(i);
            open("/dev/null", O_RDWR);
        }
    }
    dup(0);
    dup(0);
    umask(0); 
    return 0;
}

