#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    long type;
    char str[0];
}MSG;


int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("usages: %s key type \n", argv[0]);
        exit(1);
    }

    key_t key = atoi(argv[1]);
    long type = atoi(argv[2]);

    //获得指定的消息队列
    int msq_id;
    if ( (msq_id = msgget(key, 0777)) < 0)
    {
        perror("msgget error");
    }
    printf("msq id: %d\n", msq_id);

    //从消息队列中接受指定类型的消息

    MSG *m = (MSG*)malloc(sizeof(MSG) + 13);
    if (msgrcv(msq_id, m, sizeof(MSG) + 13 -sizeof(long), type, IPC_NOWAIT) < 0)
    {
        perror("msgrcv error");
    }
    else
    {
        printf("type: %ld char*str = %s", m->type, m->str);
    }

    return 0;
}
