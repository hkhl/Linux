#ifndef __ACCOUT_H__
#define __ACCOUNT_H__

#include <pthread.h>
#include <semaphore.h>

typedef struct
{
    int code;            //账户
    double balance;      //金额
    int semid;           //在共享资源上绑定一个信号量集
}Account;



//取款
extern double withdraw(Account *a, double amt);
//存款
extern double deposit(Account *a, double amt);
//查看余额
extern double get_balacne(Account *a);


#endif
