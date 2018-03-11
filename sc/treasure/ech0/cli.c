/*************************************************************************
	> File Name: cli.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月12日 星期二 20时33分22秒
 ************************************************************************/
#include "sck.h"

int sockfd;
void sckClient_init(void **handler)
{
    signal(SIGUSR1, *handler);
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket");

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connect
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("connect");

    struct sockaddr_in localaddr;
    memset(&localaddr, 0, sizeof(localaddr));
    socklen_t len = sizeof(localaddr);
    if(getsockname(sockfd, (struct sockaddr *)&localaddr, &len) < 0)
        ERR_EXIT("getsockname");
    printf("ip = %s  port = %d\n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port));
}
int main()
{
    sckClient_init(&handler);
    pid_t id = fork();

    if(id < 0)
        ERR_EXIT("fork");
    if(id == 0)
    {
        cs_send(sockfd);
    }
    if(id > 0)
    {
        int ret = cs_recv(sockfd);  //返回是0的时候对方关闭,, 需要关闭退出子进程
        if(ret == 0)
        {
            kill(id, SIGUSR1);
        }
    }
    close(sockfd);
    return 0;
}
