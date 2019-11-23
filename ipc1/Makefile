PROGS = pipe1 pipe2 popen1

all: $(PROGS)


%:%.c
	gcc -I../include -o $@ $<  -L../lib -lapue 