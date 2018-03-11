/*************************************************************************
	> File Name: 10M.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月14日 星期日 12时52分00秒
 ************************************************************************/
/* 创建一个10M的文件 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    int fd = open("./10M.txt", O_CREAT|O_WRONLY, 0644);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

    if(lseek(fd, 1024*1024*10-1, SEEK_SET) == -1)
    {
        perror("lseek");
        exit(1);
    }

    write(fd, "1", 1);

    close(fd);
    return 0;
}



