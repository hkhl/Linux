/*************************************************************************
	> File Name: terminal_ioctl.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月14日 星期日 10时20分34秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<fcntl.h>

int main(void)
{
    struct winsize size;
    if(isatty(STDOUT_FILENO) == 0)
        exit(1);
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0)
    {
        perror("ioctl TIOCGWINSZ error");
        exit(1);
    }
    printf("%d rows, %d column\n", size.ws_row, size.ws_col);
    return 0;
}
