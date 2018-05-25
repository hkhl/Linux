#ifndef __MSG_H__
#define __MSG_H__
#include <sys/types.h>

typedef struct
{
    //协议头
    char head[10];
    char checknum; //校验码

    //协议体
    char buff[512]; //数据

}Msg;




/*
 * 发送一个自定义协议的message
 * 发送数据存放在buff中
 */

extern int write_msg(int sockfd, char *buff, size_t len);

/*
 * 读取一个自定义协议的message
 * 读取数据存放在buff中
 */
extern int read_msg(int sockfd, char *buff, size_t len);

#endif

