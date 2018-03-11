/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月22日 星期二 19时53分38秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#define PORT 4000
void *recvpthread(void *arg)
{
    int sfd = *(int *)arg;
    char buf[1024];
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        int l = recv(sfd, buf, sizeof(buf), 0);
        if(l < 0)
            exit(1);
        else if(l == 0)
            exit(1);
        else
            printf("Recv: %s", buf);
    }
    return NULL;
}
void *sendpthread(void *arg)
{
    int sfd = *(int *)arg; 
    char buf[1024];
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));
        send(sfd, buf, strlen(buf), 0);
    }
    return NULL;
}
int main()
{
    int sfd;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1)
    {
        perror("sfd");
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //htonl
    if(connect(sfd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
    {
        perror("connect");
        exit(1);
    }

    pthread_t pth1, pth2;
    pthread_create(&pth1, NULL, recvpthread, &sfd);
    pthread_create(&pth2, NULL, sendpthread, &sfd);
    pthread_join(pth1, NULL);
    //pthread_join(pth2, NULL);   //发送会阻塞 等待输入  所以不等待这个线程
    close(sfd);
    return 0;
}

