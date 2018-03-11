/*************************************************************************
	> File Name: 非阻塞读终端.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月13日 星期六 21时48分19秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

#define MSG_TRY "tre again\n"

int main(void)
{
    char buf[10];
    int fd, n;
    /* 获取flags */
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    /* 更改flags  */
    flags |= O_NONBLOCK;
    /* 设置flags */
    if(fcntl(STDIN_FILENO, F_SETFL, flags) == -1)
    {
        perror("fcntl");
        exit(1);
    }

    tryagain:
    n = read(STDIN_FILENO, buf, sizeof(buf));
    if (n < 0)
    {
        if(errno == EAGAIN)        /* 当读到的内容(终端输入)为空时 */
        {
            sleep(2);
            write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
            goto tryagain;
        }
        perror("read /dev/tty");  /* 当fd为不存在的值, 会出现读出错(非EAGAIN) */
        exit(1);
    }
    write(STDOUT_FILENO, buf, n); 
    close(n);
    return 0;
}
