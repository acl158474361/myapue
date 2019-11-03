#include "apue.h"

int main(int argc,char* argv[]){
	char* ptr;

	for(;ptr=*__environ;++__environ){
		printf("environ: %s\n",ptr);
	}
	return 0;
}
