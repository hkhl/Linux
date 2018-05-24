#include "account.h"
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


//取款
double withdraw(Account *a, double amt)
{
    assert(a != NULL);

    if (amt < 0 || amt > a->balance)
        return 0.0;

    double balance = a->balance;
    sleep(1);
    balance -= amt;
    a->balance = balance;

    return amt;
}
//存款
double deposit(Account *a, double amt)
{
    assert(a != NULL);
    if (amt < 0)
        return 0.0;
    double balance = a->balance;
    sleep(1);
    balance += amt;
    a->balance = balance;

    return amt;
}
//查询余额
double get_balacne(Account *a)
{
    assert(a != NULL);

    double balance = a->balance;

    return balance;
}
