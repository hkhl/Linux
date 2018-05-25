# $@代表目标  $^代表所有依赖  $<代表依赖中第一个
CC = gcc
CPPFLAGS = -Iinclude
CFLAGS = -g -Wall
LDFLAGS = -lpthread

src = $(wildcard ./src/*.c)
obj = $(patsubst %.c,%.o,$(src))

all = client server
srvobj = $(filter-out ./src/client.o, $(obj))
cliobj = $(filter-out ./src/server.o, $(obj))

.PHONY:target
target: client server
server:$(srvobj)
	$(CC) $^ $(LDFLAGS) -o $@
client:$(cliobj)
	$(CC) $^ $(LDFLAGS) -o $@
%.o:%.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@


#server:server.o msg.o
#	$(CC) $^ $(LDFLAGS) -o $@
#client:client.o msg.o
#	$(CC) $^ $(LDFLAGS) -o $@
#需要当前目录的.o文件, 当前.o文件需要src的.c文件
#%.o:src/%.c
#	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

#- 表示执行错误 依然执行后面的命令
#@ 忽略命令, 只打印内容
.PHONY:clean
clean:
	-rm -f *.o
	-rm -f ./src/*.o
	-rm -f $(all)
.PHONY:file
file:
	@echo $(cliobj)
	@echo $(obj)
