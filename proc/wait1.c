#include "pr_exit.h"
#include "apue.h"

int main(void){
    pid_t pid;
    int status;

    if( (pid=fork())<0 )
        err_sys("fork error");
    else if(pid == 0)
        exit(7);
    if(waitpid(pid,&status,0)<0)
        err_sys("waitpid error");
    pr_exit(status);

    if( (pid=fork())<0 )
        err_sys("fork error");
    else if(pid == 0)
        abort();
    if(waitpid(pid,&status,0)<0)
        err_sys("waitpid error");
    pr_exit(status);



    if( (pid=fork())<0 )
        err_sys("fork error");
    else if(pid == 0)
        status/=0;
    if(waitpid(pid,&status,0)<0)
        err_sys("waitpid error");
    pr_exit(status);
}