target=app
src = $(wildcard *.c)    
obj = $(patsubst %.c,%.o,$(src))

CPPFLAGS = -Iinclude
CFLAGS = -g -Wall
LDFALGS = 
CC = gcc

$(target):$(obj)
	$(CC) $^ $(LDFALGS) -o $@
%.o:%.c
	$(CC) -c $^  $(CPPFLAGS) $(CFLAGS) -o $@

.PHONY:clean
clean:
	-rm -f *.o
	-rm -f $(target)
.PHONY:file
file:
	@echo $(src)
