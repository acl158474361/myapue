PROGS = pollmsg sendmsg bindunix

all: $(PROGS)

%:%.c
	gcc -I../include -o $@ $< -L../lib -lapue -lpthread