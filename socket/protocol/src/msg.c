#include "msg.h"
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>


static unsigned char  msg_check(Msg* message)
{
    unsigned char s = 0;
    for (int i = 0; i < sizeof(message->head); i++)
        s += message->head[i];

    for (int i = 0; i < sizeof(message->buff); i++)
        s += message->buff[i];

    return s;
}


/*
 * 发送一个自定义协议的message
 * 发送数据存放在buff中
 */
int write_msg(int sockfd, char *buff, size_t len)
{
    Msg message;
    memset(&message, 0, sizeof(message));
    strcpy(message.head, "iotek2012");
    memcpy(message.buff, buff, len);
    message.checknum = msg_check(&message);

   if (write(sockfd, &message, sizeof(message)) != sizeof(message))
       return -1;
   return sizeof(message);
}

/*
 * 读取一个自定义协议的message
 * 读取数据存放在buff中
 */
int read_msg(int sockfd, char *buff, size_t len)
{
    Msg message;
    memset(&message, 0, sizeof(message));

    size_t size;
    if ( (size = read(sockfd, &message, sizeof(message))) < 0 )
        return -1;
    else if (size == 0)
        return 0;
    unsigned char s = msg_check(&message);

    if (s == (unsigned char)message.checknum
            && !strcmp("iotek2012", message.head))
    {
        memcpy(buff, message.buff, len);
        return sizeof(message);
    }
    return -1;
}


