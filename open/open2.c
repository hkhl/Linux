/*************************************************************************
	> File Name: 1.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月14日 星期日 12时29分52秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    /* 文件不存在, 创建文件, 若文件存在, 报错 */
    int fd = open("/home/wang/itcast.txt", O_WRONLY|O_CREAT|O_EXCL, 0664);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }
    write(fd, "花前月下iii", strlen("花前月下iii"));
    close(fd);
    return 0;
}
