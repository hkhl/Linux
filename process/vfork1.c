#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
    int num = 100;
    pid_t id  = vfork();
    if(id > 0)
    {
        printf("parent:%d num = %d\n", getpid(), num);
        //exit(0);
        return 0;
    }
    if(id == 0)
    {
        printf("child:%d num = %d\n", getpid(), num);
        exit(0);
        //return 0;

    }
    printf("main: %d\n", getpid());
    //exit(0);
    //return 0;
}

