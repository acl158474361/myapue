#include "apue.h"
#define MAXSIZE 10
int main(){
	
	char buffer[MAXSIZE];
	fgets(buffer,MAXSIZE,stdin);
	printf("buffer的内容：%s ",buffer);	 
}
