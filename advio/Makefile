PROGS = nonblockw mandatory rot13a rot13b

all:$(PROGS)
%:%.c
	gcc -I../include -o $@ $<  -L../lib -lapue  -lrt

clean:
	rm $(PROGS)