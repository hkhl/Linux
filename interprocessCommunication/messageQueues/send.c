#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>


typedef struct {
    long type;     //消息类型
    char str[0];     //消息本身
    //int s;
}MSG;

/*
 * 往消息队列发送消息
 */
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage : %s key\n", argv[0]);
        exit(1);
    }


    key_t key = atoi(argv[1]);
    //key_t key = IPC_PRIVATE;    //键默认为0x00000
    //key_t key = ftok(argv[1], 0);  //通常传入路径 路径不能重复

    printf("key: %d\n", key);

    //创建消息队列
    int msq_id;
    if ( (msq_id = msgget(key, IPC_CREAT|IPC_EXCL|0777)) < 0)
    {
        perror("msgget error");
    }
    printf("msq id : %d\n", msq_id);


    //定义发送的消息
    char sendbuf[] = "hello world!";
    MSG *m1 = (MSG* )malloc(sizeof(MSG) + sizeof(sendbuf));
    m1->type = 1;
    sprintf(m1->str, "%s", sendbuf);

    //发送消息到消息队列
    if (msgsnd(msq_id, m1, sizeof(MSG) + sizeof(sendbuf)-sizeof(long), IPC_NOWAIT) < 0)
    {
        perror("msgsnd error");
    }

    free(m1);
    return 0;
}
