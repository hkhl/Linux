/*************************************************************************
	> File Name: waitpid.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月10日 星期日 10时20分24秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<errno.h>
#include<signal.h>

int main()
{
    int id = 0;
    for(int i = 0; i < 4; i++)
    {
        id = fork();
        if(id == 0)
        {
            printf("子进程%d号开始  %d\n", i+1,   getpid());
            //exit(0);
        }
    }


    int mmpid;
    //waitpid()  参数用WNOHANG的时候 
    //非阻塞 一直检测子进程状态
    //子进程没有结束 不停的返回0  
    //若子进程结束  返回子进程ID
    //没有子进程 返回-1
    while((mmpid = waitpid(-1, NULL, WNOHANG)) != -1)   //>= 0
    {
        if(mmpid > 0)
        {
            printf("等待退出的子进程的id: %d\n", mmpid);
        }
    }


    //判断退出和信号中断
   /* while((mmpid = waitpid(-1, NULL, 0)) < 0)
    {
        if(errno == EINTR)
        {
            continue;
        }
        break;
    }*/

    /*
    while(1)
    {
        mmpid = wait(NULL);
        if(mmpid == -1)
        {
            if(errno == EINTR)
                continue;
            break;
        }
        printf("等待退出的子进程的id: %d\n", mmpid);
    }*/
        
    printf("父进程退出...\n");

    while(1)
    {
        pause();
    }
    return 0;
}
