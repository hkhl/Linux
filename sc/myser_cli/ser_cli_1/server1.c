/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月22日 星期二 19时14分38秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define PORT 4000
#define backlog 10
//只能有一个客户端连接 ,
//并且当这个客户端退出时,还是只能有一个客户端连接

pthread_mutex_t mut;

int status = 0;
struct ps
{
    int sfd;
    pthread_t *th;
};

void *recvpthread(void *arg)
{
    struct ps *p = (struct ps *)arg;
    int sfd = *(int *)arg;
    char buf[1024];
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        int l = recv(sfd, buf, sizeof(buf), 0);
        if(l <= 0)
            break;
        else
        printf("Recv: %s", buf);
    }
    pthread_mutex_lock(&mut);
    status = 0;
    pthread_mutex_unlock(&mut);
    pthread_cancel(*(p->th));       //一个线程内结束另外一个线程
                                    //被cancel的线程内部没有使用互斥锁 所以不会死锁
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
    int sfd, cfd;
    struct sockaddr_in addr;
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        perror("sfd");
        exit(1);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);    
    addr.sin_addr.s_addr = htonl(INADDR_ANY);       
   
    //地址重复利用
    int on = 1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    {
        perror("addres");
        exit(1);
    }
    if(bind(sfd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if(listen(sfd, backlog) == -1)
    {
        perror("listen");
        exit(1);
    }
    struct sockaddr_in client_addr;
    pthread_t pth1, pth2;

    while(1)
    {
        socklen_t len = sizeof(client_addr);
        memset(&client_addr, 0, sizeof(client_addr));
        if((cfd = accept(sfd, (struct sockaddr *) &client_addr, &len)) == -1)
        {
            perror("accept");
            exit(1);
        }
        pthread_mutex_lock(&mut);
        status++;
        pthread_mutex_unlock(&mut);
        if(status > 1)
        {
            close(cfd);
            continue;
        }
        printf("CLient IP: %s\n", inet_ntoa(client_addr.sin_addr));   //addr.sin_addr;
        
        struct ps ps1;
        ps1.sfd = cfd;
        ps1.th = &pth2;

        //线程的可分离和不可分离性
        //此代码不可分离
        //当客户端连接 创建线程 退出时,线程没了 但是资源还占用着
        //没用join 不释放 一直调用 一直资源累计
        pthread_create(&pth1, NULL, recvpthread, &ps1);
        pthread_detach(pth1);           //设置线程为可分离
        pthread_create(&pth2, NULL, sendpthread, &cfd);
        pthread_detach(pth2);
    }
    close(sfd);
    close(cfd);
    return 0;
}
