/*************************************************************************
	> File Name: 阻塞读终端.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月13日 星期六 21时38分59秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>


int main()
{
    char buf[10];
    int n = read(STDIN_FILENO, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, n);

    return 0;
}
