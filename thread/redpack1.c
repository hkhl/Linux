/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月19日 星期二 19时29分44秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>

float money;   //红包金额
int n;         //红包数目
int mark;      //红包剩余个数
pthread_mutex_t mut; //互斥锁

struct INFO
{
    float rmoney;
    pthread_t thread;
}info[5];

int flag = 1;  //标记第flag个抢红包的人


void *writemoney(void)
{
    money = 100.0;     // 设置红包金额
    mark = n = 3;      // 设置红包数量
    pthread_exit(NULL);
}
void *readmoney(void *arg)   
{
    int *pth = (int *)arg;

    if(money == 0 || mark == 0)    //money == 0 或者 n = 0
    {
        for(int i = 0; i < 5; i ++)  //结束其他线程
        {
            if(info[i].thread != *pth)
            pthread_cancel(info[i].thread);
        }
        printf("该红包已被撤销\n");                                              /////////////////有时候不打印这句话?
    }
    else if(money != 0 && mark!= 0)
    {
        //生成随机两位小数
        time_t ts;
        srand((unsigned int)time(&ts)+*pth);
    
        float readmoney = rand()%(10000/(n-1))/100.0; //范围0.00-24.99; 不除以n的原因是红包范围过于平均
        if(readmoney == 0.00)
        {
            readmoney = 0.01;     //随机红包的最低金额
        }

        pthread_mutex_lock(&mut);
      
        //抢红包逻辑处理 
        if(mark > 1)
        {
            printf("id :[%lu] 抢了%.2f元", pthread_self(), readmoney);
            info[flag++].rmoney = readmoney;
            money -= readmoney;
            mark--;                        //红包个数减1
            printf("剩余%.2f元\n", money);
        }
        if(mark == 1)  //剩下一个红包, 谁抢给谁, 不看随机金额
        {
            readmoney = money;
            printf("id :[%lu] 抢了%.2f元", pthread_self(), readmoney);
            info[flag++].rmoney = readmoney;
            money -= readmoney;
            mark--;                        //红包个数减1
            printf("剩余%.2f元\n", money);
        }

        //红包金额为0
        //if(money == 0)
        {
          //  printf("很不幸 , 红包已经被抢光了\n");
        }
        pthread_mutex_unlock(&mut);
    }
    pthread_exit(NULL);
}
int main()
{
    int temp;

    if((temp = pthread_create(&info[0].thread, NULL, (void *)writemoney, NULL)) == 0)   //写数据
    {
        printf("线程write创建成功\n");
    }

    sleep(1);
    //for循环创建线程?
    int nth = 4;//线程数量
    for(int i = 1; i <= nth; i++)                           /////////////////总是最后一个线程抢不到?
    {
        if((temp = pthread_create(&info[i].thread, NULL, (void *)readmoney, &info[i].thread)) == 0) 
        {
            printf("线程read%d创建成功\n", i);//gettid()获取内核线程id号
        }
    }

    printf("等待线程结束\n");
    for(int i = 0; i < nth+1; i++)
    //pthread_detach(thread[i]);
    pthread_join(info[i].thread, NULL);

    printf("线程结束\n");

    
    printf("/*********抢红包信息*********/\n");
    float max = 0.0;
    int x;
    for(int i = 1; i <= nth; i++)
    {
        if(info[i].rmoney > max)
        {
            max = info[i].rmoney;
            x = i;
        }
        if(info[i].rmoney == 0.00)    
        {
            printf("线程%d手慢无\n", i);
        }
        else
        {
            printf("线程%d抢了%.2f元\n", i, info[i].rmoney);
        }
    }
    printf("/**********运气王**********/\n");
    printf("线程%d   抢了%.2f元\n", x, max);

    return 0;
}
