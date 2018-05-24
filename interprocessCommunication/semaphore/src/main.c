#include "pv.h"
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int semid = I(1, 1);
    printf("semid = %d\n", semid);
    if(semid < 0)
    {
        perror("semid I error");
        exit(1);
    }

    int i = 3;

    pid_t pid = fork();
    if (pid > 0)
    {
        P(semid, 0, 1);
        i++;
        V(semid, 0, 1);

        D(semid);
        wait(0);
    }
    else if (pid < 0)
    {
        P(semid, 0, 1);
        i++;
        V(semid, 0, 1);
        //D(semid);
    }


    printf("i = %d\n", i);

    exit(0);
}
