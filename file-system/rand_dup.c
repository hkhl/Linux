/*************************************************************************
	> File Name: rand_dup.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年02月14日 星期三 10时43分39秒
 ************************************************************************/
/* 从文件中读取1000个随机数,排序, 写到另外一个文件当中  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    //生成随机数
    int fd1 = open("number1.txt", O_CREAT|O_RDWR, 0766);

    dup2(fd1,  STDOUT_FILENO);
    int num;

    srand((unsigned int)time(NULL));

    for(int i = 0; i < 1000; i++)
    {
        num = rand()%1000+1;   //1-1000;
        printf("%d\n", num);
    }
    
    //close(fd);

    return 0;
}
int main2()
{
    //dup
    close(STDOUT_FILENO);
    int fd = open("number.txt", O_CREAT|O_RDWR, 0766);
    int num;

    srand((unsigned int)time(NULL));

    for(int i = 0; i < 1000; i++)
    {
        num = rand()%1000+1;   //1-1000;
        printf("%d\n", num);
        write(STDOUT_FILENO, "123\n", 4);
    }
    
    close(fd);

    return 0;
}

int main1()
{
    /* c函数写入文件 */
    srand((unsigned int)time(NULL));

    FILE *fp = fopen("number.txt", "a");
    int num;

    for(int i = 0; i < 1000; i++)
    {
        num = rand()%1000+1;   //1-1000;
        fprintf(fp, "%d\n", num);
    }


    fclose(fp);
    return 0;
}

