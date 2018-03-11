/*************************************************************************
	> File Name: 重定向.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月14日 星期日 13时03分50秒
 ************************************************************************/

/* 将printf重定向到某个文件中 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    int ofd = dup(STDOUT_FILENO);   //ofd = 3 功能是标准输出
    close(STDOUT_FILENO);           //文件描述副1被关闭

    int fd = open("output.txt", O_CREAT|O_RDWR, 0644);   //此时打开fd = 1, 因为无人占用1
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }
    fd = dup(ofd);     //  ??    不加这行代码, fd和STDOUT_FILENO(宏定义)都代表1,  1指向了打开的output文件
    //                           加这行代码, fd(4)指向终端, STDOUT_FILENO(1) 指向打开的output文件 

    write(fd, "123", 3);
    write(ofd, "\nttt:123", 8);
    write(STDOUT_FILENO, "456", 3);

    printf("\nfd = %d\n", fd);
    printf("hello ! How are you ? I'm fine! And you ? I'm ok!\n");

    //close(fd);

    return 0;
}

