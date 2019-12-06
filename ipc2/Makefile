PROGS = pollmsg sendmsg bindunix send_recv.o open.o opend.o main_client main_server

all: $(PROGS)

%:%.c
	gcc -I../include -Wall -DLINUX -D_GNU_SOURCE -o $@ $< -L../lib -lapue -lpthread

%.o:%.c
	gcc -c -I../include  -Wall -DLINUX -D_GNU_SOURCE -o $@ $< 

main_client: main_client.o send_recv.o open.o
	gcc -Wall -DLINUX -D_GNU_SOURCE -o main_client main_client.o send_recv.o open.o -L../lib -lapue
main_server: main_server.o send_recv.o opend.o
	gcc -Wall -DLINUX -D_GNU_SOURCE -o main_server main_server.o send_recv.o opend.o -L../lib -lapue

clean:
	rm $(PROGS)