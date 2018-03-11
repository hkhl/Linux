/*************************************************************************
	> File Name: stat.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年01月22日 星期一 12时50分08秒
 ************************************************************************/
/* 可以用来验证文件或者目录是否存在 */
#include <stdio.h>
#include <sys/stat.h>

int main()
{
    struct stat stbuf;
    if(stat("..", &stbuf) == -1)
    {
        perror("stat");
        return 1;
    }
    else
    {
        printf(" exist! ");
    }

    return 0;
}
