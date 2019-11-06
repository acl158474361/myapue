PROGS = detach.o Tpthread_attr
CC =  gcc

all: $(PROGS)

%:%.c 
	$(CC) -I../include -o $@ $< -L../lib -lapue -lpthread -lbsd

%.o:%.c
	$(CC) -c -I../include -o $@ $< -L../lib -lapue -lpthread -lbsd