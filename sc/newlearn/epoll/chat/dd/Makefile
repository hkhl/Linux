.PHONY:all
all:1 2 
1:server.c
	gcc -g chat.h server.c -o server
2:cli.c
	gcc -g chat.h cli.c -o client

.PHONY:clean
clean:
	rm -rf 1 2
