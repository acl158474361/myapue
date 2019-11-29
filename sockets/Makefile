PROGS = getaddrinfo connect_retry1.o connect_retry2.o initserver.o\
 ruptime ruptimed

all: $(PROGS)

%:%.c
	gcc -I../include -o $@ $< -L../lib -lapue

%.o:%.c
	gcc -I../include -c -o $@ $<

ruptime: connect_retry2.o ruptime.o 
	gcc -I../include -o $@ connect_retry2.o ruptime.o -L../lib -lapue

ruptimed: initserver.o ruptimed.o
	gcc -I../include -o $@ initserver.o ruptimed.o -L../lib -lapue