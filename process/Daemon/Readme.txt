

root 权限运行

当我一普通用户执行a.out时，进程表中并没有出现新创建的守护进程，但当我以root用户执行时，成功了，并在/目录下创建了daemon.
log文件，cat查看后确实每个一分钟写入一次。为什么只能root执行，那是因为当我们创建守护进程时，已经将当前目录切换我/目录，
所以当我之后创建daemon.log文件是其实是在/目录下，那肯定不行，因为普通用户没有权限，或许你会问那为啥没报错呢？其实是有出错，
只不过我们在创建守护进程时已经将标准输入关闭并重定向到/dev/null，所以看不到错误信息。








//调用守护进程API

#include <unistd.h>

int daemon(int nochdir, int noclose);

DESCRIPTION 
The daemon() function is for programs wishing to detach themselves from 
the controlling terminal and run in the background as system daemons.

If nochdir is zero, daemon()  changes  the  process’s  current  working 
directory to the root directory ("/"); otherwise,

If  noclose is zero, daemon() redirects standard input, standard output 
and standard error to /dev/null; otherwise,  no  changes  are  made  to 
these file descriptors. 

          功能：创建一个守护进程

          参数：

          nochdir：=0将当前目录更改至“/”

          noclose：=0将标准输入、标准输出、标准错误重定向至“/dev/null”

          返回值：

          成功：0

          失败：-1
