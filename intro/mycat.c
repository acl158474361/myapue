#include "apue.h"
#define BUFFSIZE 4096
int main(){
	int n;
	char buffer[BUFFSIZE];
	while ( (n=read(STDIN_FILENO,buffer,BUFFSIZE))>0 )
		if (write(STDOUT_FILENO,buffer,n)!=n)
			err_sys("write error");

	if (n<0)
		err_sys("read error");
	exit(0);
}
