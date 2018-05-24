#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tell.h"

static int fd[2];

//管道初始化
void init()
{
    if (pipe(fd) < 0)
        perror("pipe error");

}

//利用管道进行等待
void wait_pipe()
{//利用管道阻塞读
    char ch;
    if(read(fd[0], &ch, 1) < 0)
        perror("erad");

}

//利用管道进行通知
void notify_pipe()
{
    char ch = 'q';
    write(fd[1], &ch, 1);
}

//销毁管道
void destroy_pipe()
{
    close(fd[0]);
    close(fd[1]);
}

