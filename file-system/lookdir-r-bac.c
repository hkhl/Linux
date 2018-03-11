/*************************************************************************
	> File Name: lookdir-r-bac.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年01月23日 星期二 10时10分31秒
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#define MAX_LINE 1024
void dirwalk(char *dir, void (*fcn)(char *))
{
    DIR *dr;
    struct dirent *dt;
    char name[MAX_LINE];
    
    if ( (dr = opendir(dir)) == NULL)
    {
        perror("opendir");
        exit(1);
    }

    while ( (dt = readdir(dr)) != NULL)
    {
        if (strcmp(dt->d_name, ".") == 0 || strcmp(dt->d_name, "..") == 0)
            continue;
        //if((dt->d_type & DT_DIR) != 0)
        {
            sprintf(name, "%s/%s", dir, dt->d_name);
            (*fcn)(name);
        }
    }
    closedir(dr);
}

void fsize(char *name)
{
    struct stat stbuf;
    if (stat(name, &stbuf) == -1)
    {
        perror("stat");
        exit(1);
    }
    else
    {
        if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        {
            dirwalk(name, fsize);
        }
        printf("%8ld     %s\n", stbuf.st_size,  name);
    }
}

int main(int argc , char **argv)
{
    if(argc == 1)
    {
        fsize(".");
    }
    else
    {
        while (--argc > 0)
        {
            fsize(*++argv);
        }
    }

    return 0;
}
