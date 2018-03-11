#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>


int main(int argc, char *argv[])
{

    union sigval mysigval;
    mysigval.sival_int = 222;

    pid_t pid = atoi(argv[1]);
    if(sigqueue(pid ,SIGINT, mysigval))
    perror("sigqueue");
    printf("send success");

    return 0;
}
