/*************************************************************************
	> File Name: module2.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月18日 星期一 14时44分39秒
 ************************************************************************/

#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WANGDONG");
MODULE_DESCRIPTION("This module provide a function to module2");
int str_len(const char *str)
{
    int count = 0;
    while(*str != '\0')
    {
        count++;
        str++;
    }
    return count;
}

static int init_mod(void)
{
    printk(KERN_INFO "Hello, this module was installed successfully!\n");
    return 0;
}

static void exit_mod(void)
{
    printk(KERN_INFO "Goodbue, the module was uninstalled!\n");
}

EXPORT_SYMBOL(str_len);
module_init(init_mod);
module_exit(exit_mod);


