PROGS = critical intwaitpid pr_mask.o reenter signal.o sigusr sleep1.o sleep2.o tsleep2 mask suspend1 suspend2 charactertime wait_tell.o\
system.o tsystem
CC = gcc 

all: $(PROGS)

%:%.c
	gcc  -I../include -o $@ $< -L../lib -lapue

%.o:%.c
	gcc -c -I../include -o $@ $< -L../lib -lapue

tsleep2: tsleep2.o sleep2.o
	gcc -o tsleep2 tsleep2.o sleep2.o -L../lib -lapue

mask: mask.o signal.o pr_mask.o
	gcc -o mask mask.o signal.o pr_mask.o -L../lib -lapue

suspend1: suspend1.o signal.o pr_mask.o
	gcc -o suspend1 suspend1.o signal.o pr_mask.o -L../lib -lapue

suspend2: suspend2.o signal.o pr_mask.o 
	gcc -o suspend2 suspend2.o signal.o pr_mask.o -L../lib -lapue
charactertime: charactertime.o  wait_tell.o pr_mask.o 
	gcc -o charactertime charactertime.o wait_tell.o pr_mask.o -L../lib -lapue
tsystem: tsystem.o system.o
	gcc -o tsystem tsystem.o system.o

 
clean:
	rm $(PROGS) tsleep2.o mask.o suspend1.o suspend2.o charactertime.o