/*************************************************************************
	> File Name: lookdir-r.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年01月18日 星期四 10时57分55秒
 ************************************************************************/
/* 递归遍历目录 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define MAX_PATH 1024


void dirwalk(char *dir, void (*fcn)(char *))
{
    char name[MAX_PATH];
    struct dirent *dp;
    DIR *dfd;

    if ( (dfd = opendir(dir)) == NULL)
    {
        perror("opendir");
        exit(1);
    }

    while ( (dp = readdir(dfd)) != NULL)
    {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;
        if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
        {
            fprintf(stderr, "dirwalk: name too long\n");
        }
        else
        {
            sprintf(name, "%s/%s", dir, dp->d_name);
            (*fcn)(name);            
        }
    }
    /* 读取记录向出错 */
    /*if (errno == appropriately)
    {
        perror("readdir");
        exit(1);
    }*/
    

}

void fsize(char *name)
{
    struct stat stbuf;

    if (stat(name, &stbuf) == -1)
    {
        perror("stat");
        exit(1);
    }
    if ( (stbuf.st_mode & S_IFMT) == S_IFDIR)
    {
        dirwalk(name, fsize);
    }
    printf("%8ld %s\n", stbuf.st_size, name);
}

int main(int argc, char **argv)
{
    if (argc == 1)
        fsize(".");
    else
        while(--argc > 0)
        fsize(*++argv);
    return 0;
}
