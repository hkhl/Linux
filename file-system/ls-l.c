/*************************************************************************
	> File Name: ls-l.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年02月07日 星期三 21时23分39秒
 ************************************************************************/
/* 实现ls -l功能 */

/* man 2 stat中*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

#define MAX_LINE 1024

void PrintFileType(struct dirent *dt)
{
    if((dt->d_type & DT_BLK) == DT_BLK)
    {
        putchar('b');
    }
    else if((dt->d_type & DT_CHR) == DT_CHR)
    {
        putchar('c');
    }
    else if((dt->d_type & DT_DIR) == DT_DIR)
    {
        putchar('d');
    }
    else if((dt->d_type & DT_FIFO) == DT_FIFO)
    {
        putchar('f');
    }
    else if((dt->d_type & DT_REG) == DT_REG)
    {
        putchar('-');
    }
    else if((dt->d_type & DT_LNK) == DT_LNK)
    {
        putchar('l');
    }
    else if((dt->d_type & DT_SOCK) == DT_SOCK)
    {
        putchar('s');
    }
    else
    {
        putchar('u');
    }
}

void PrintFileMod(struct dirent *dt)
{
    struct stat stbuf;

    if(stat(dt->d_name, &stbuf) == -1)
    {
        perror("stat");
        exit(1);
    }
    if((stbuf.st_mode & S_IRWXU) == S_IRUSR + S_IXUSR + S_IWUSR)
    {
        printf("rwx");
    }
    else if((stbuf.st_mode & S_IRWXU) == S_IRUSR + S_IWUSR)
    {
        printf("rw-");
    }
    
    if ((stbuf.st_mode & S_IRWXG) == S_IRGRP + S_IWGRP + S_IXGRP)
    {
        printf("rwx");
    }
    else if ((stbuf.st_mode & S_IRWXG) == S_IRGRP + S_IWGRP)
    {
        printf("rw-");
    }
    if((stbuf.st_mode & S_IRWXO) == S_IROTH + S_IXOTH)
    {
        printf("r-x");
    }
    else if((stbuf.st_mode & S_IRWXO) == S_IROTH)
    {
        printf("r--");
    }


 /*S_IRWXU     00700   owner has read, write, and execute permission
           S_IRUSR     00400   owner has read permission
           S_IWUSR     00200   owner has write permission
           S_IXUSR     00100   owner has execute permission

           S_IRWXG     00070   group has read, write, and execute permission
           S_IRGRP     00040   group has read permission
           S_IWGRP     00020   group has write permission
           S_IXGRP     00010   group has execute permission

           S_IRWXO     00007   others  (not  in group) have read, write, and
                               execute permission
           S_IROTH     00004   others have read permission
           S_IWOTH     00002   others have write permission
           S_IXOTH     00001   others have execute permission
*/
}

void PrintFileHardLink(struct dirent *dt)
{
    struct stat stbuf;
    if(stat(dt->d_name, &stbuf) == -1)
    {
        perror("stat");
        exit(1);
    }

    printf("   %ld  " , stbuf.st_nlink);
}

void List(char *dir)
{
    int IsDir = 0;
    char realname[256];
    struct stat stbuf;
    /*int stat(const char *pathname, struct stat *buf);*/
    if(stat(dir, &stbuf)  == -1)
    {
        perror("stat");
        exit(1);
    }


    if((stbuf.st_mode & S_IFMT) == S_IFDIR)
    {
        //打开的是目录
        IsDir = 1;
    }
    else
    {
        //打开的是非目录
        IsDir = 0;
        char *bac_dir = dir; 

        int i = 0;
        int flag = 0;
        while(*bac_dir++)
        {
            i++;
            if(*bac_dir == '/')
            {
                flag = i;
            }
        }

        if(flag == 0)
        {
            strcpy(realname, dir);
            dir = ".";
        }
        else
        {
            strcpy(realname, dir+flag+1);
            bac_dir = dir;
            for(int i = 0; i < flag+1; i++)
            {
                dir[i] = bac_dir[i];
            }
            dir[flag+1] = '\0';
            
        }
    //printf("\nflag = %d   dir = [%s]   realname = [%s] \n", flag, dir, realname);
}

    //printf("\n %d \n", stbuf.st_uid);             //uid 1000 代表什么. ..等等  /etc/passwd

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
        if (IsDir == 0 && strcmp(dt->d_name, realname) != 0)
        {
            continue;
        }
        else
        {
            if (strcmp(dt->d_name, ".") == 0 || strcmp(dt->d_name, "..") == 0)
            continue;

            PrintFileType(dt);

            PrintFileMod(dt);

            PrintFileHardLink(dt);

            sprintf(name, "  %s", dt->d_name);
            printf("%s  ", name);
            printf("\n");
        }
    }
    closedir(dr);
}

int main(int argc, char **argv)
{
    if(argc == 1)
    {
        List(".");
    }
    else
    {
        while(--argc)
        {
            List(*++argv);
        }
    }
    return 0;
}

