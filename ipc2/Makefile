PROGS = pollmsg sendmsg bindunix send_recv.o

all: $(PROGS)

%:%.c
	gcc -I../include -o $@ $< -L../lib -lapue -lpthread

%.o:%.c
	gcc -c -I../include -o $@ $< 

clean:
	rm $(PROGS)