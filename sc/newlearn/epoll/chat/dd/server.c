/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月29日 星期日 23时23分30秒
 ************************************************************************/
/*
 *
struct Protocol
{
    int id;             //协议号
    int fd;             //fd
    char src[20];       //发送方
    char dest[20];      //接收方
    char data[1024];    //数据内容
};
 * */
#include"chat.h"
int x;
int main()
{
    FILE *f = fopen("./name.txt", "w");
    fclose(f);

    char connect = 'n';  //y/n

    struct Protocol packet[OPEN_MAX];       //维护在线队列
    int client[OPEN_MAX];
    int number = 0;                         //在线人数
    int i, j, on, maxi, listenfd, connfd, sockfd;
    int nready , efd, res; 
    ssize_t n;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];        /* #define INET_ADDRSTRLEN 16 */

    socklen_t clilen;

    struct epoll_event tep, ep[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        ERR_EXIT("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    on = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");

    if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
        ERR_EXIT("bind");

    if(listen(listenfd, 20) < 0)
        ERR_EXIT("listen");

    for(i = 0; i < OPEN_MAX; i++)
        client[i] = -1;           
    maxi = -1;

    if( (efd = epoll_create(OPEN_MAX)) < 0)
        ERR_EXIT("epoll_create");

    tep.events = EPOLLIN; tep.data.fd = listenfd;
    if( (res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep)) < 0)
        ERR_EXIT("epoll_ctl");

    while(1)
    {
        if( (nready = epoll_wait(efd, ep, OPEN_MAX, -1)) < 0)   /* -1阻塞 */
            ERR_EXIT("epoll_wait\n");

        for(i = 0; i < nready; i++)
        {
            if(!(ep[i].events & EPOLLIN))    /* 如果不是这个EPOLLIN响应 */
                continue;
            if(ep[i].data.fd == listenfd)
            {
                clilen = sizeof(cliaddr);

                if( (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen )) < 0 )
                    ERR_EXIT("accept");
                printf("client: IP:%s  PORT:%d\n", inet_ntop(AF_INET,&cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));

                for(j = 0; j < OPEN_MAX; j++)
                {
                    if(client[j] < 0)
                    {
                        client[j] = connfd;
                        break;
                    }
                }
                if(j == OPEN_MAX)
                    ERR_EXIT("too many clients");
                if(j > maxi)
                    maxi = j;

                /* 设置刚刚返回的connfd,监控读事件 */
                tep.events = EPOLLIN; tep.data.fd = connfd;
                if( (res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep)) < 0)
                    ERR_EXIT("epoll_ctl");
            }
            else
            {
                sockfd = ep[i].data.fd;
                if( (n = read(sockfd, &packet[number] ,sizeof(packet))) == 0)
                {   //读取出错
                    for(j = 0; j <= maxi; j++)
                    {
                        if(client[j] == sockfd)
                        {
                            client[i] = -1;
                            break;
                        }
                    }
                    if( (res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL)) < 0)
                        ERR_EXIT("epoll_ctl");

                    close(sockfd);
                    printf("client[%d] closed connection\n", j);
                    /*离线包处理*/
                }
                else
                {
                    packet[number].fd = sockfd;

                    if(packet[number].id == 1)
                    {
                        //将名字写入文件


                        FILE *pf = fopen("./name.txt", "a+");
                        if(pf == NULL)
                            return 0;
                        fprintf(pf, "%s\n", packet[number].src);
                        fclose(pf);
                    }
                    else if(packet[number].id == 2)
                    {
                        int n;
                        //转发数据包

                        //广播包
                        if(strlen(packet[number].dest) == 0 && strlen(packet[number].data) > 0)
                        {
                            for(int i = 0; i < OPEN_MAX; i++)
                            {
                                if(client[i] != -1 && client[i] != sockfd)
                                {
                                    write(client[i], &packet[number] , sizeof(packet));
                                }
                            }
                            memset(packet[number].data, 0, sizeof(packet[number].data));                        
                        }

                        //如果接收到的数据包只是名字dest,内容为空, 则进行判断,是否有这个人
                        else if(strlen(packet[number].data) == 0)
                        {
                            for(n = 0; n < number+1; n++)
                            {
                                if(packet[n].id != 1)
                                    continue;
                                if(strcmp(packet[n].src, packet[number].dest) == 0)
                                {
                                    connect = 'y';
                                    write(packet[number].fd, "y", 1);
                                    break;
                                }
                            }
                        }
                        
                        if(connect == 'y')
                        {
                            for(n = 0; n < number+1; n++)
                            {
                                if(packet[n].id != 1)
                                    continue;
                                if(strcmp(packet[number].dest, packet[n].src) == 0)
                                {
                                    write(packet[n].fd, &packet[number], sizeof(packet));
                                    //聊天记录先存入文件 再清空
                                    memset(packet[number].data, 0, sizeof(packet[number].data));
                                    break;
                                }

                            }
                        }
                       // else
                        {
                            write(packet[number].fd, "n", 1);
                            //没有这个人, 发送错误包 id=-1
                            //packet[number].id = -1;
                            //memset(packet[number].data, 0, sizeof(packet[number].data));
                            //sprintf(packet[number].data, "没有此人!\n");
                            //write(packet[number].fd, &packet[number], sizeof(packet));


                            //向这个人发送没有这个好友

                        }
                    }
                }
            }
        }
        number++;
       
        printf("******************\n");
        printf("包的信息如下:\n");
        for(int l  = 0; l < number; l++)
            printf("[id=%d] [src=%s] [dest=%s] [data=%s] [sockfd=%d] [oack.fd=%d]***\n", packet[l].id, packet[l].src, packet[l].dest, packet[l].data, sockfd, packet[l].fd);
        printf("******************\n");
        for(int i = 0; i < 10; i++)
        printf("client[%d]: %d\n", i, client[i]);
        printf("******************\n");
        
    }

close(listenfd);
close(efd);
return 0;
}
