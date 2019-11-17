PROGS = init.o lockfile.o set_cloexec.o single.o reread reread2

all: $(PROGS)

%.o:%.c
	gcc -I../include -c -o $@ $<  


%:%.c
	gcc -I../include -o $@ $< -L../lib -lapue -lpthread 


reread:  single.o reread.o lockfile.o
	gcc -o reread  single.o reread.o  lockfile.o -L../lib -lapue -lpthread

reread2: single.o reread2.o lockfile.o
	gcc -o reread  single.o reread.o  lockfile.o -L../lib -lapue -lpthread


clean:
	rm $(PROGS)