PROGS = exec fork1 fork2  printuid racecondition interp system.o pr_exit.o systest1

all: ${PROGS}
%:%.c
	gcc -I../include -o $@ $@.c -L../lib -lapue

%.o:%.c
	gcc -c -I../include -o $@ $(subst .o,,$@).c -L../lib -lapue

systest1: pr_exit.o system.o systest1.o
	gcc -I../include -o systest1 pr_exit.o system.o systest1.o -L../lib -lapue