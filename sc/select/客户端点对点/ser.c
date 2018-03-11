/*************************************************************************
	> File Name: ser.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月31日 星期四 19时33分04秒
 ************************************************************************/
//创建个结构体 .
//先是多客户端连接
//先达到广播的效果
//服务器发送给所有客户端
//接着实现客户端发过来 服务器广播
//接着实现 客户端发过来 服务器发给定点客户端
//  然后呢  同一个聊天室

#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#define PORT 8000
#define BACKLOG 10
#define CLISIZE 20

struct info
{
    int cfd;
    char id[10];
};
/*
void message(int cfd)
{
    char sendbuf[1024];
    char recvbuf[1024];

    fd_set rset;
    FD_ZERO(&rset);
    int nready;
    int maxfd;

    int fd_stdin = fileno(stdin);

    if(fd_stdin > cfd)
        maxfd = fd_stdin;
    else
        maxfd = cfd;

   // while(1)
    //{
        FD_SET(fd_stdin, &rset);
        FD_SET(cfd, &rset);
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        
        if(nready == -1)
        perror("select");
        if(nready == 0)
        {
            printf("超时\n");
            return ;
        }

        if(FD_ISSET(cfd, &rset))
        {
            int len;
            if((len = recv(cfd, recvbuf, sizeof(recvbuf), 0))== -1)
            {
                perror("recv");
            }
            else if(len == 0)
            {
                printf("peer close\n");
                exit(1);
            }
            else
            {
                printf("recf:%s\n", recvbuf);
            }
            memset(recvbuf, 0, sizeof(recvbuf));
        }*/
/*
        if(FD_ISSET(fd_stdin, &rset))
        {
            fgets(sendbuf, sizeof(sendbuf), stdin);
            if(send(cfd, sendbuf, strlen(sendbuf), 0) == -1)
            {
                perror("send");
            }
            memset(sendbuf, 0, sizeof(sendbuf));
        }
    }
}*/
int main()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1)
    perror("socket");

    struct sockaddr_in addr, cli_addr;
    memset(&addr, 0, sizeof(addr));
    memset(&cli_addr, 0, sizeof(cli_addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int on = 1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("setsockopt");
    }
    if(bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    perror("bind");
    if(listen(sfd, BACKLOG) == -1)
        perror("listen");

    socklen_t len = sizeof(cli_addr);



    //select
    fd_set sckset;
    FD_ZERO(&sckset);
    int sck[CLISIZE] = { 0 };
    memset(sck, -1, sizeof(sck));

    int std_in = fileno(stdin);
    sck[0] = sfd;
    sck[1] = std_in;
    
    int maxfd = sfd>std_in ? sfd:std_in;

    int nready;
    int cfd;
    int i = 0;

    while(1)
    {
        //注册select
        FD_ZERO(&sckset);
        for(int i = 0; i < CLISIZE; i++)
        {
            if(sck[i] >= 0)
            {
                FD_SET(sck[i], &sckset);
            }
            if(sck[i] > maxfd)
            {
                maxfd = sck[i];
            }
        }
        printf("maxfd:%d\n", maxfd);

        nready = select(maxfd+1, &sckset, NULL, NULL, NULL);
        if(nready == 0)
        {
            printf("timeout\n");
            break;
        }
        else if(nready == -1)
        {
            perror("select\n");
            break;
        }
        else //FD_ISSET(sfd, &sckset)
        { 
            for(int i = 0; i < CLISIZE; i++)
            {

                //printf("sck: %d\n", sck[i]);
                if(sck[i] < 0)
                    continue;
                else if(sck[i] == sfd && FD_ISSET(sck[i], &sckset))//处理连接
                {
                    if((cfd = accept(sck[i], (struct sockaddr *)&cli_addr, &len))   == -1)
                    perror("accept");
                    printf("IP:%s  PORT:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

                    //连接的cfd放进数组中 
                    int j;
                    for(j = 0; j < CLISIZE; j++)
                    {
                        if(sck[j] == -1)
                        {
                            sck[j] = cfd;
                            break;
                        }
                    }
                    if(j == CLISIZE)
                    {
                        printf("full\n");
                        close(cfd);
                    }
                }
                //试试放在同层次  可以
                else   //处理连接
                {
                    for(int i = 2; i < CLISIZE; i++)
                    {
                        if(sck[i] > 0 && FD_ISSET(sck[i],&sckset))
                        {
                            char buf[1024];
                            memset(buf,'\0',sizeof(buf));
                            ssize_t size = read(sck[i],buf,sizeof(buf)-1);
                            if(size == 0)
                            {
                                printf("clien release\n");
                                close(sck[i]);
                                sck[i] = -1;
                            }
                            else if(size > 0)
                            {
                                buf[size] = '\0';
                                printf("client: %s",buf);
                            }

                            //else
                            {
                              //  perror("read");
                                //exit(EXIT_FAILURE);
                            }
                        }
                        //发送数据
                    
                    }
                }
                        if(sck[i] == std_in && FD_ISSET(sck[i],&sckset))
                        {
                            char sendbuf[1024];
                            memset(sendbuf,'\0',sizeof(sendbuf));
                            scanf("%s", sendbuf);

                            for(int i = 2; i < CLISIZE; i++)
                            {
                                if(sck[i] > 0)
                                {
                                    int ret = send(sck[i], sendbuf, strlen(sendbuf),0);
                                    if(ret < 0)
                                    {
                                        perror("send");
                                    }
                                    else if(ret > 0)
                                    {
                                        printf("send success");
                                    }
                                }
                            }
                        }
            }
        }
    }
    //close(cfd);
    close(sfd);
    return 0;
}
