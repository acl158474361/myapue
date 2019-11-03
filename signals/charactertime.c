#include "apue.h"
#include "wait_tell.h"
#include "pr_mask.h"


static void charactertime(char *str);

int main(void){ 
    pid_t pid;
    TELL_WAIT();
    
    if((pid = fork())<0){
        err_sys("fork error");
    }else if(pid == 0){
        charactertime("child output\n");
        TELL_PARENT(getppid());
    }else{
        WAIT_CHILD();
        charactertime("parent output\n");
    }
    exit(0);


}


static void charactertime(char *str){
    char *ptr;
    char c;
    setbuf(stdout,NULL);
    for(ptr = str;(c=*ptr++)!='\0';)
        putc(c,stdout);
}


