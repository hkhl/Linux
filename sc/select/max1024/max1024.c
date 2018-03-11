/*************************************************************************
	> File Name: max1024.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月29日 星期日 12时48分25秒
 ************************************************************************/
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>


#define ERR_EXIT(x)\
    perror(x);\
    exit(EXIT_FAILURE);

int main(void) 
{
    int sock[1024*2];
    int i = 0;

    printf("%d", FD_SETSIZE);
/*
    for(int i = 0; i < 1024*2; i++)
    {
        if(sock[i] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)<0)
        ERR_EXIT("socket");


        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(8000);
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if(connect(sock[i], (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        {
            printf("connect fail\n");
            sleep(2);
            ERR_EXIT("connect");
        }
        printf("%d\n", i+1);
    }
*/

    return 0;
}

