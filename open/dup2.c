/*************************************************************************
	> File Name: 11.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月14日 星期日 13时29分17秒
 ************************************************************************/

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  

int main(int argc,char*argv[])  
{  
    int fd=open("out", O_CREAT|O_RDWR, 0777);  

    /* 文件描述符的赋值此时outfd和STDOOUT_FILENO指向同一个内核文件表项 */ 
    int outfd = dup(STDOUT_FILENO);   //outfd 复制 stdout


    //close(STDOUT_FILENO);
    /*标准输出的重定向，此时进程文件表项中STDOUT_FILENO的文件指针发生变化，
    * 变为和fd的文件指针指向相同的一个内核文件表项，
    * new_fd是新的文件描述符，STDOUT_FILENO(1)是对nwe_fd大小的设置 */
    int xfd = dup2(fd, STDOUT_FILENO);   //将STDOUT_FILENO 重定向到 fd 
    
    /* 输出到终端 */
    write(outfd, "terminal: world\n",16);  

    /* 以下全部输入到文件当中 */
    write(fd, "hello\n",6);  
    write(xfd, "hello111\n",9);  
    write(STDOUT_FILENO, "world\n",6);  
    printf("11111111\n");
    printf("11111111\n");

    close(fd);
    close(outfd);
    return 0;
}  
