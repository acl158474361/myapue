PROGS = pollmsg sendmsg

all: $(PROGS)

%:%.c
	gcc -I../include -o $@ $< -L../lib -lapue -lpthread