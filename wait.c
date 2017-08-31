/*************************************************************************
	> File Name: wait.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月31日 星期四 10时18分47秒
 ************************************************************************/
//wait 和waitpid  
//wait(NULL)  和 非NULL
//以及一些宏的使用(判断子进程如何退出的 wait参数status非NULL获取子进程的一些信息)
//处理一些异常处理,比方等到所有子进程结束父进程再结束
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main()
{
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
    }
    else if(id == 0)
    {
        sleep(2);
        printf("This is child\n");
        exit(0);
        //abort();  //非正常退出
    }
    printf("This is parent\n");

    
    int res;
    int status;
    //res = waitpid(-1, &status, 0);   // -1  wait for any child process 
    //res = waitpid(pid, &status, 0);  // >0  wait for the child whose process ID is equal to the value of pid
    //res = waitpid(0, &status, 0);    // 0   wait for any child process whose *process group ID* is equal to that of the calling process
    //res = waitpid(-5, &status, 0);   // <-1 wait for any child process whose process group ID is equal to the *absolute* value of pid.
    res = wait(&status);


    /*参数:    可以获得等待子进程的信息*/
    /*返回值:  成功返回等待子进程的ID, 通常是结束的子进程 */
    /*调用wait后, 父进程进入等待状态, 直到一个子进程结束 */
    //The call wait(&status) is equivalent to: waitpid(-1, &status, 0);




    if(WIFEXITED(status))
    {
        printf("The child terminated normally    %d \n", WEXITSTATUS(status));
    }
    else if(WIFSIGNALED(status))
    {
        printf("the child process was terminated by a signa  %d \n", WTERMSIG(status));
    }
    else if(WIFSTOPPED(status))
    {
        printf("the child process was stopped by delivery of a signa  %d\n", WSTOPSIG(status));
    }
    /*  man 2 wait 
        WIFEXITED(status)
              returns true if mZe child terminated normally, that is, by calling exit(3) or _exit(2), or by returning from main().
        WEXITSTATUS(status)

        WIFSIGNALED(status)
              returns true if the child process was terminated by a signal.
        WTERMSIG(status)

        WIFSTOPPED(status)
              returns  true if the child process was stopped by delivery of a signal; this is possible only if the call was done using WUNTRACED or when the child
              is being traced (see ptrace(2)).
        WSTOPSIG(status)
    */


    /*等待多个子进程的退出, 不等待会产生僵尸进程*/
    /*
    wait(): on success, returns the process ID of the terminated child; on error, -1 is returned.
    while(1)
    {
        ret = wait(NULL);
        if(ret == -1)
        {
            if(error == EINTR)
            {
                continue;
            }
            break;
        }
    }


     */
    return 0;
}
