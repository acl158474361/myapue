PROGS = pipe1 pipe2 popen1 myuclc grep1  add 

all: $(PROGS)


%:%.c
	gcc -I../include -o $@ $<  -L../lib -lapue 