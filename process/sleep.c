/*************************************************************************
	> File Name: 1.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月02日 星期六 12时24分33秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>

void handler(int num)
{
    printf("recv:%d\n", num);
}
int main()
{
    int n = 0;
    signal(SIGINT, handler);
    do
    {
        n = sleep(10);
        printf("还有%d秒没睡\n", n);

    }while(n > 0);

    printf("wanle");
    return 0;
}
