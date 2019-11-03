#include "apue.h"
static void first_handler(void);
static void second_handler(void);


int main(int argc,char* argv[]){
	if(atexit(second_handler)!=0)
		err_sys("can't register second_handler");
	if(atexit(first_handler)!=0)
		err_sys("can't register first_handler");
	if(atexit(first_handler)!=0)
		err_sys("can't register first_handler");
	printf("main is done\n");
	return 0;

}

static void first_handler(void){
	printf("first_handler \n");

}

static void second_handler(void){
	printf("second_handler \n");
}
