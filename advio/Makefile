PROGS = nonblockw mandatory

all:$(PROGS)
%:%.c
	gcc -I../include -o $@ $<  -L../lib -lapue 

clean:
	rm $(PROGS)