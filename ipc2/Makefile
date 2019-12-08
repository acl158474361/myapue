PROGS = pollmsg sendmsg bindunix send_recv.o open.o opend.o main_client main_server main_client1 main_server1

all: $(PROGS)

%:%.c
	gcc -g -I../include -Wall -DLINUX -D_GNU_SOURCE -o $@ $< -L../lib -lapue -lpthread

%.o:%.c
	gcc  -g -c -I../include  -Wall -DLINUX -D_GNU_SOURCE -o $@ $< 

main_client: main_client.o send_recv.o open.o
	gcc -g -Wall -DLINUX -D_GNU_SOURCE -o main_client main_client.o send_recv.o open.o -L../lib -lapue
main_server: main_server.o send_recv.o opend.o
	gcc  -g -Wall -DLINUX -D_GNU_SOURCE -o main_server main_server.o send_recv.o opend.o -L../lib -lapue

main_client1: main_client.o send_recv.o open1.o unix_socket.o
	gcc -g -Wall -DLINUX -D_GNU_SOURCE -o main_client1 main_client.o send_recv.o open1.o unix_socket.o -L../lib -lapue
main_server1: main_server1.o send_recv.o opend1.o unix_socket.o
	gcc  -g -Wall -DLINUX -D_GNU_SOURCE -o main_server1 main_server1.o send_recv.o opend1.o unix_socket.o -L../lib -lapue

clean:
	rm $(PROGS)