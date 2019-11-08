PROGS = detach.o Tpthread_attr timeout getenv.o getenv_r.o getenv_r2.o 
CC =  gcc

all: $(PROGS)

%:%.c 
	$(CC) -I../include -o $@ $< -L../lib -lapue -lpthread 

%.o:%.c
	$(CC) -c -I../include -o $@ $<  -L../lib -lapue -lpthread

timeout: detach.o timeout.o
	$(CC) -o timeout detach.o timeout.o -L../lib -lapue -pthread

clean:
	rm -rf $(PROGS) timeout.o