/*************************************************************************
	> File Name: 不可重入函数.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月07日 星期二 21时27分36秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>


void setTimer(int seconds, int mseconds)
{
    struct timeval temp;
    temp.tv_sec = seconds;
    temp.tv_usec = mseconds;
    select(0, NULL, NULL, NULL, &temp);
    printf("111\n");
}

int main()
{
    while(1)
    {
        malloc(sizeof(int));
        setTimer(0, 1);
    }
    return 0;
}
