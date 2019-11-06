PROGS = detach.o Tpthread_attr timeout
CC =  gcc

all: $(PROGS)

%:%.c 
	$(CC) -I../include -o $@ $< -L../lib -lapue -lpthread 

%.o:%.c
	$(CC) -c -I../include -o $@ $<  

timeout: detach.o timeout.o
	$(CC) -o timeout detach.o timeout.o -L../lib -lapue -pthread

clean:
	rm -rf $(PROGS) timeout.o