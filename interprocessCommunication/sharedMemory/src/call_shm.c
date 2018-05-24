#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include "tell.h"

int main(void)
{
    //创建共享内存
    int shmid;
    if ( (shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT|IPC_EXCL|0777)) < 0)
    {
        perror("shmget error");
        exit(1);
    }

    pid_t pid;
    init();     //初始化管道

    if ( (pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid > 0)
    {//parent process
        //共享内存映射
        int *pi = (int *)shmat(shmid, 0, 0);
        if (pi == (int *)-1)
        {
            perror("shmat error");
            exit(1);
        }
        //向共享内存中写入数据
        *pi = 100;
        *(pi+1) = 200;

        //解除共享内存
        shmdt(pi);
        //写完毕后通知子进程 (进程间同步)
        notify_pipe();
        destroy_pipe();
        wait(0);
    }
    else
    {//child process
        //子进程阻塞, 等待父进程先往共享内存中写入数据
        wait_pipe();
        /*子进程从共享内存中读取数据*/
        //子进程共享内存映射
        int *pi = (int *)shmat(shmid, 0, 0);
        if (pi == (int *)-1)
        {
            perror("shmat error");
            exit(1);
        }

        printf("*pi = %d\n*(pi+1) = %d\n", *pi, *(pi + 1));

        //解除共享内存
        shmdt(pi);
        //删除共享内存
        shmctl(shmid, IPC_RMID, NULL);
        destroy_pipe();
    }
    return 0;
}
