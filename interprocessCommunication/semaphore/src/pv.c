#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pv.h"

//自己定义联合体
union semun
{
    int      val;
    struct   semid_ds    *buf;
    unsigned short       *array;
};

//创建信号量集并初始化sennums个信号灯/信号量的值value
int I(int semnums, int value)
{
    //创建信号量集
    int semid = semget(IPC_PRIVATE, semnums, IPC_CREAT | IPC_EXCL | 0777);
    if (semid < 0)
        return -1;

    union semun un;
    unsigned short *array = (unsigned short*)calloc(semnums, sizeof(unsigned short));

    for (int i = 0; i < semnums; i++)
        array[i] = value;

    un.array = array;

    /*
     * 初始化信号量集中所有信号灯的初值
     * 0:表示要初始化的所有信号灯
     */

    if (semctl(semid, 0, SETALL, un) < 0)
    {
        perror("semctl error");
        return -1;
    }
    free(array);
    return semid;
}

//对信号量集(semid)中的信号灯(semnum)做P操作
void P(int semid, int semnum, int value)
{

    assert(value >= 0);
    /*
     * 定义sembuf类型的结构体数组, 放置若干个结构体变量
     * 对应要操作的信号量, 要做的P或者V操作
     */
    struct sembuf ops[] = {{semnum, -value, SEM_UNDO}};
    if (semop(semid, ops, sizeof(ops)/sizeof(struct sembuf)) < 0)
    {
        perror("semop1 error");
    }
}

//对信号量集(semid)中的信号灯(semnum)做V操作
void V(int semid, int semnum, int value)
{
    assert(value >= 0);
    struct sembuf ops[] = {{semnum, value, SEM_UNDO}};
    if (semop(semid, ops, sizeof(ops)/sizeof(struct sembuf)) < 0)
    {
        perror("semop2 error");
    }
}

//销毁信号量集(semid)
void D(int semid)
{
    if(semctl(semid, 0, IPC_RMID, NULL) < 0)
    {
        perror("semct3 error");
    }
}


