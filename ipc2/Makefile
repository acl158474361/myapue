PROGS = pollmsg sendmsg bindunix send_recv.o 

all: $(PROGS)

%:%.c
	gcc -I../include -Wall -DLINUX -D_GNU_SOURCE -o $@ $< -L../lib -lapue -lpthread

%.o:%.c
	gcc -c -I../include  -Wall -DLINUX -D_GNU_SOURCE -o $@ $< 

clean:
	rm $(PROGS)