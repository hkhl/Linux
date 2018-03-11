/*************************************************************************
	> File Name: rm.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年02月14日 星期三 09时53分14秒
 ************************************************************************/

/* rmdir  unlink  递归遍历目录 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void deep(char *dir, void(*f)(char *))
{
    DIR *dr;
    struct dirent *dt;
    char name[1024];
    
    if((dr = opendir(dir)) == NULL)
    {
        perror("opendir");
        exit(1);
    }

    while((dt = readdir(dr)) != NULL)
    {
        if(strcmp(dt->d_name, ".") == 0 || strcmp(dt->d_name, "..") == 0)
        {
            continue;
        }
        sprintf(name, "%s/%s", dir, dt->d_name);
        printf("[%s]\n", dt->d_name);
        (*f)(name);
    }
    closedir(dr);
}

void fun(char *name)
{
    struct stat stbuf;

    if(stat(name, &stbuf) == -1)
    {
        perror("stat");
        exit(1);
    }

    if((stbuf.st_mode & S_IFMT) == S_IFDIR)
    {
        //进入目录,
        //删除目录
        deep(name, fun);
        rmdir(name);
    }
    //else
    {
        //删除文件
        unlink(name);
    }

}

int main(int argc, char **argv)
{
    if(argc == 1)
    {
        printf("是否要删除当前目录所有内容?(y/n)");
        char ch = getchar();
        if(ch != 'y')
        {
            exit(0);
        }
        fun(".");
    }
    else
    {
        while(--argc)
        {
            fun(*++argv);
        }
    }
    return 0;
}
