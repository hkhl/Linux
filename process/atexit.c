/*************************************************************************
	> File Name: atexit.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月25日 星期五 21时53分19秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void a(void)
{
    printf("1I am a\n");
}

void b(void )
{
    printf("2I am b\n");
}

int main()
{
    //先调用的后执行
    atexit(a);
    printf("hello ...\n");
    atexit(b);
    
    
    //当调用了_exit()函数, atexit()将不在执行
    _exit(0);
    
    //当调用以下函数 , 依然会执行
    // return 0;
    //exit(0);
}
