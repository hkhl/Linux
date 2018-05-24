#include "account.h"
#include "pv.h"
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    //在共享内存中创建银行账户
    int shmid;
    if ( (shmid = shmget(IPC_PRIVATE, sizeof(Account),
                    IPC_CREAT|IPC_EXCL|0777)) < 0)
    {
        perror("shmget error");
        exit(1);
    }
    //进程共享内存映射
    Account *a = (Account *)shmat(shmid, 0, 0);
    if (a == (Account *)-1)
    {
        perror("shmat error");
        exit(1);
    }
    a->code = 1000001;
    //存款1000
    printf("balance = %.2f\n", deposit(a, 1000));


    //创建信号量初始化一个信号量, 初值为1
    if( (a->semid = I(1, 1)) < 0)
    {
        perror("init I  error");
        exit(1);
    }


    pid_t pid;
    if ( (pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid > 0)  //parent process
    {//父进程取款
        double amt = withdraw(a, 1000);
        printf("pid %d withdraw %f form code %d\n", getpid(), amt, a->code);
        wait(0);

        //销毁信号量
        D(a->semid);
        //解除共享内存映射
        shmdt(a);
        //删除共享内存
        shmctl(shmid, IPC_RMID, NULL);
    }
    else
    {//child process
        //子进程取款
        double amt = withdraw(a, 1000);
        printf("pid %d withdraw %f form code %d\n", getpid(), amt, a->code);
        printf("result : %f\n", a->balance);
        //解除共享内存映射
        shmdt(a);
    }

    //printf("result : %f\n", a->balance);   //error 对共享内存操作要在释放之前
    return 0;
}
