#include <sys/shm.h>
#include <sys/sem.h>
//#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



//进程间同步问题,  对同一块共享内存进行读写
//atm是进程间户斥问题
typedef struct
{
    int val;
    int semid;
}Storage;

void init(Storage *s)
{
    assert(s != NULL);
    //创建信号量集(2个信号量)

    if ( (s->semid = semget(IPC_PRIVATE, 2, IPC_CREAT|IPC_EXCL|0777)) < 0)
    {
        perror("semget error");
        exit(1);
    }

    //对信号量集中的所有信号量初始化
    union semun
    {
        int               val;
        struct semid_ds   *ds;
        unsigned short *array;
    };
    union semun un;
    //2个信号量初值设置为0
    unsigned short array[2] = {0, 0};
    un.array = array;

    if (semctl(s->semid, 0, SETALL, un) < 0)
    {
        perror("semctl error");
        exit(1);
    }
}
void destroy(Storage *s)
{
    assert(s != NULL);
    if (semctl(s->semid, 0, IPC_RMID, NULL) < 0)
    {
        perror("semctl error");
        exit(1);
    }
}

void write(Storage *s, int val)
{
    //写入数据到STotage
    s->val = val;
    printf("%d write %d\n", getpid(), val);

    //设置0号信号量(s1)作V(1)操作
    struct sembuf ops_v[1] = {{0, 1, SEM_UNDO}};
    //设置1号信号量(s2)作P(1)操作
    struct sembuf ops_p[1] = {{1, -1, SEM_UNDO}};


    //V(s1)
    if (semop(s->semid, ops_v, 1) < 0)
    {
        perror("semop error");
    }
    //P(s2)
    if (semop(s->semid, ops_p, 1) < 0)
    {
        perror("semop error");
    }
}


void read(Storage *s)
{
    assert(s != NULL);

    //设置0号信号量(s1)作P(1)操作
    struct sembuf ops_p[1] = {{0, -1, SEM_UNDO}};
    //设置1号信号量(s2)作V(1)操作
    struct sembuf ops_v[1] = {{1, 1, SEM_UNDO}};


    //P(s1)
    if (semop(s->semid, ops_p, 1) < 0)
    {
        perror("semop error");
    }

    //从Stroage中读取数据
    printf("%d read %d\n", getpid(), s->val);

    //V(s2)
    if (semop(s->semid, ops_v, 1) < 0)
    {
        perror("semop error");
    }
}


int main()
{
    //将共享资源Storage创建在共享内存中
    int shmid;
    if ( (shmid = shmget(IPC_PRIVATE, sizeof(Storage), IPC_CREAT|IPC_EXCL|0777)) < 0)
    {
        perror("shmget error");
        exit(1);
    }

    //父进程进行共享内存映射
    Storage *s = (Storage *)shmat(shmid, 0, 0);
    if (s == (Storage *)-1)
    {
        perror("shmat error");
        exit(1);
    }

    //创建信号量 并初始化
    init(s);
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (pid > 0)
    {
        int i = 1;
        for(; i <= 100; i++)
            write(s, i);

        wait(0);
        destroy(s);
        shmdt(s);
        shmctl(shmid, IPC_RMID, NULL);
    }
    else
    {
        int i = 1;
        for(; i <= 100; i++)
            read(s);
        shmdt(s);
    }


    exit(0);
}
