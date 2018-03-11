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
    /* 追加写, 文件必须存在, 不存在执行程序没有任何效果 */
    int fd = open("/home/wang/itcast.txt", O_APPEND|O_WRONLY, 0664);
    /* 追加写, 文件不存在, 则创建文件 */
    //int fd = open("/home/wang/itcast.txt", O_APPEND|O_WRONLY|O_CREAT, 0664);
    /*  文件不存在, 则创建文件, 文件存在, 则截断文件内容为0开始写 */
    //int fd = open("/home/wang/itcast.txt", O_TRUNC|O_WRONLY|O_CREAT, 0664);
    write(fd, "花前月下!@", strlen("花前月下!@"));
    close(fd);
    return 0;
}
