/*************************************************************************
	> File Name: mkfifo.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年02月09日 星期五 11时45分03秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{

//int mkfifo（const char *pathname, mode_t mode）;
    mkfifo("./fifo", 0777);

    return 0;
}
