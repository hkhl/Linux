/*************************************************************************
	> File Name: alarm.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月02日 星期六 13时09分58秒
 ************************************************************************/
//kill函数

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
void handler(int num)
{
    printf("SIGALRM信号:  %d\n", num);
    alarm(1);           
    //间接递归     handler ---> alarm  ---> 内核 ---> handler
}
int main()
{
    
    if(signal(SIGALRM, handler) == SIG_ERR)
    {
        perror("signal");
        return 0;
    }
   // while(1)
    {
        alarm(1);
     //   sleep(1);
    }
    while(1)
    {
        pause();
        printf("****\n");
    }
    printf("end\n");
    return 0;
}
