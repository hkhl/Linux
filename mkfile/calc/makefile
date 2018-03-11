#$(wildcard *.c) 找到当前目录所有.c文件
#$(patsubst %.c,%.o,a.c)  将a.c 替换为 .o文件
# $@代表目标  $^代表所有依赖  $<代表依赖中第一个
#

target=app
src = $(wildcard *.c)    
obj = $(patsubst %.c,%.o,$(src))


# gcc 头文件  编译方式 动态库(链接的时候加载)
CC = gcc
CPPFLAGS = -Iinclude
CFLAGS = -g -Wall
LDFALGS = 

$(target):$(obj)
	$(CC) $^ $(LDFALGS) -o $@

%.o:%.c
	$(CC) -c $^  $(CFLAGS) $(CPPFLAGS) -o $@



#- 表示执行错误 依然执行后面的命令
#@ 忽略命令, 只打印内容
.PHONY:clean
clean:
	-rm -f *.o
	-rm -f $(target)
.PHONY:file
file:
	@echo $(src)
