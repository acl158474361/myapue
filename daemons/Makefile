PROGS = init.o single.o  

all: $(PROGS)

%.o:%.c
	gcc -I../include -c -o $@ $<  -lpthread


%:%.c
	gcc -I../include -o $@ $< -L../lib -lapue -lpthread 

clean:
	rm $(PROGS)