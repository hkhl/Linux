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
    close(STDOUT_FILENO);
    int fd = open("output.txt", O_CREAT|O_RDWR, 0644);   
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }
    printf("%d\n", fd);
    printf("hello ! How are you ? I'm fine! And you ? I'm ok!\n");

    //close(fd);  //加上这句话将写不到文件当中
    return 0;
}

