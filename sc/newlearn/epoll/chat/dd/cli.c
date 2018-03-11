/*************************************************************************
	> File Name: cli.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月31日 星期四 19时04分54秒
 ************************************************************************/
/*
 *协议:   id = 1   登录包
               2   数据包
               -1  错误信息
               -2  离线包
struct Protocol
{
    int id;             //协议号
    int fd;             //fd
    char src[20];       //发送方
    char dest[20];      //接收方
    char data[1024];    //数据内容
};
  */
#include"chat.h"

void message(int n, int sfd);

void menu(int sfd)
{
    int n = 0;
    printf("1.......群聊\n");
    printf("2.......私聊\n");
    printf("3.......查看好友聊\n");
    printf("0.......退出\n");
    printf("请输入您的选择: ");
    scanf("%d", &n);
    getchar();

    while(n != 1 || n != 2 || n != 3)
    {
        switch(n)
        {
            case 0: exit(0);
            break;
            case 1: message(n, sfd);
            break;
            case 2: message(n, sfd);
            break;
            case 3: message(n, sfd);
            break;
            default: printf("请选择已存在选项: ");
            break;
        }
        scanf("%d", &n);
        getchar();
    }

}



struct Protocol packet;
void message(int sel, int sfd)
{
    char connect; //标志是否创建对话成功
    int len;      //read函数返回长度
    if(sel == 3)
    {
        char name[50][20];
        int n = 0;
        FILE *pf = fopen("./name.txt", "r");
        if(pf == NULL)
        exit(0);


        while(!feof(pf))
        {
            fscanf(pf, "%s\n", name[n]);
            n++;
        }
        printf("当前在线人数:%d人\n", n);
        for(int i = 0; i < n; i++)
        {
            printf("%s\n", name[i]);
        }
        fclose(pf);
        menu(sfd);
    }


    struct Protocol sendbuf;
    struct Protocol recvbuf;
    sendbuf.id = 1;
    recvbuf.id = 1;

    fd_set rset;
    FD_ZERO(&rset);
    int nready;
    int maxfd;
    int fd_stdin = fileno(stdin);
    if(fd_stdin > sfd)
        maxfd = fd_stdin;
    else
        maxfd = sfd;

    int i = 0;

tryagain:
    while(1)
    {
        FD_SET(fd_stdin, &rset);
        FD_SET(sfd, &rset);
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        
        if(nready == -1)
            ERR_EXIT("select");
        if(nready == 0)
            continue;

        if(FD_ISSET(sfd, &rset))
        {
            //读
            if( (len = read(sfd, &recvbuf, sizeof(recvbuf)))== -1)
                ERR_EXIT("recv");
            else if(len == 0)
                ERR_EXIT("server close");
            else
            {
                if(sel == 2 && recvbuf.id == 2 && strlen(recvbuf.data) > 0 && strcmp(recvbuf.dest, packet.src)==0) 
                {
                    printf("%s:\033[37m%s\033[0m", recvbuf.src, recvbuf.data);
                    memset(&recvbuf, 0, sizeof(recvbuf));

                }
                else if((sel == 1) && recvbuf.id == 2 && strlen(recvbuf.data) > 0 )
                {
                    printf("%s:\033[37m%s\033[0m", recvbuf.src, recvbuf.data);
                    memset(&recvbuf, 0, sizeof(recvbuf));

                }
                else if(recvbuf.id == -1 && strlen(recvbuf.data) > 0)
                {
                    printf("错误信息:%s",  recvbuf.data); 
                }
            }
            memset(&recvbuf, 0, sizeof(recvbuf));
        }

        if(FD_ISSET(fd_stdin, &rset))
        {
            if(sel == 1)
            {
                sendbuf.id = 2;
                memset(sendbuf.dest, 0, sizeof(sendbuf.dest));
                strcpy(sendbuf.src, packet.src);
                memset(sendbuf.data, 0, sizeof(sendbuf.data));
                fgets(sendbuf.data, sizeof(sendbuf.data), stdin);
                if(strcmp(sendbuf.data, "exit\n") == 0)
                    menu(sfd);
                write(sfd, &sendbuf, sizeof(sendbuf));
                memset(sendbuf.data, 0, sizeof(sendbuf.data));
                
            }
            if(sel == 2)
            {
                fgets(sendbuf.data, sizeof(sendbuf.data), stdin);
                if(strcmp(sendbuf.data, "exit\n") == 0)
                {
                    printf("\033[37m您已退出当前对话!\n\033[0m");
                    sendbuf.id = 1;
                    menu(sfd);
                }

                if(sendbuf.data[0] == '@' && sendbuf.id == 1)
                {
                    sendbuf.id = 2;
                    strcpy(sendbuf.src, packet.src);

                    int i = 1;
                    while(sendbuf.data[i] != '\n')
                    {
                        sendbuf.dest[i-1] = sendbuf.data[i];
                        i++;
                    }

                    memset(sendbuf.data, 0, sizeof(sendbuf.data));
                    memset(&connect, 0, 1);
                    write(sfd, &sendbuf, sizeof(sendbuf));
                    len = read(sfd, &connect, 1);
                    if(len == 0)
                    ERR_EXIT("close");

                    if(connect == 'n')
                    {
                        printf("\033[31m您没有这个好友呦!请重新@:\n\033[0m");
                        memset(sendbuf.dest, 0, sizeof(sendbuf.dest));
                        sendbuf.id = 1;
                        goto tryagain;
                    }
                    system("clear");
                    printf("\033[37m会话建立成功,可以发送消息!\n\033[0m");
                    memset(sendbuf.data, 0, sizeof(sendbuf.data));
                    fgets(sendbuf.data, sizeof(sendbuf.data), stdin);
                    write(sfd, &sendbuf, sizeof(sendbuf));
                    memset(sendbuf.data, 0, sizeof(sendbuf.data));
                }
                else if(sendbuf.id == 2)
                {
                    write(sfd, &sendbuf, sizeof(sendbuf));
                    memset(sendbuf.data, 0, sizeof(sendbuf.data));
                }
            }
        }
    }
}

int main()
{
    char buf[MAXLINE];
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1)
    ERR_EXIT("socket");

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(SERV_PORT);




    printf("请输入用户名:");
    printf("\033[37m");
    scanf("%s", packet.src);
    printf("\033[0m");

    if(connect(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    ERR_EXIT("connect");
    packet.id = 1;          // id = 1 登录包  id = 2 数据包
    write(sfd, &packet, sizeof(packet));

    menu(sfd);




    close(sfd);
    return 0;
}
