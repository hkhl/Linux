/*************************************************************************
	> File Name: vfork.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月25日 星期五 19时12分35秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//return 两次 报错
int main()
{
    int num = 11;
    pid_t id  = vfork();
    //char * argv[] = {"ls", "-l", NULL};
    if(id > 0)
    {
        printf("I am parent %d\n", getpid());
        //exit(0);
      
    }
    if(id == 0)
    {
        printf("I am child\n");
        printf(" %d\n", num);
        //execve("./hello.out", NULL, NULL);
        /*
        execve("/bin/ls", argv , NULL);
        printf("printf\n");*/
    
    }


    printf("waimian %d", getpid());
    exit(0); 
}
