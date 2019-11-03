#include "apue.h"
#include <sys/wait.h>

int main(){

   char buffer[MAXLINE];
   pid_t pid;
   int status;
   printf("%% ");
   while ((fgets(buffer,MAXLINE,stdin))!=NULL){
	if (buffer[strlen(buffer)-1]=='\n')
		buffer[strlen(buffer)-1]=0;
	if ((pid=fork())<0){
		err_sys("fork error");
	}else if(pid == 0){
		/* child */
		execlp(buffer,buffer,(char *)0);
		err_ret("couldn't execute %s",buffer);
		exit(127);
	}

	if ( (pid=waitpid(pid,&status,0))<0)
		err_sys("waitpid error");	
	printf("%% ");
   }
   exit(0);
	   
}
