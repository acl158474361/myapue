PROGS = threadid exitcode badexit2 cleanup mutex1.o mutex2.o mutex3.o\
rwlock.o barrier Tpthread_create

all: $(PROGS)

%:%.c
	gcc -I../include  -o $@ $< -L../lib -lapue -lpthread -lrt -lbsd

%.o: %.c
	gcc -c -I../include -o $@ $< -L../lib -lapue -lpthread


clean:
	rm -rf $(PROGS)