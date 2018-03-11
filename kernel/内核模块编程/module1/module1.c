/*************************************************************************
	> File Name: module2.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月18日 星期一 14时44分39秒
 ************************************************************************/

#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WANGDONG");
MODULE_DESCRIPTION("This module call str_len");

static char *str = "Linux";
extern int str_len(char *);

static int init_mod(void)
{
    int length = 0;
    length = str_len(str);
    printk(KERN_INFO "Hello, I am module1 !\n");
    printk(KERN_INFO "Welcome to use the str_len function !\n");
    printk(KERN_INFO "The length of string '%s' is: %d\n", str, length);
    return 0;
}

static void exit_mod(void)
{
    printk(KERN_INFO "Goodbue, the module was uninstalled!\n");
}
module_param(str, charp, S_IRUGO);
module_init(init_mod);
module_exit(exit_mod);


