#include"apue.h"
#include<sys/wait.h>

static void sig_usr(int);

int main(void){
    pid_t pid;
    if(signal(SIGUSR1,sig_usr)==SIG_ERR){
        err_sys("can't catch SIGUSR1");
    }
    if(signal(SIGUSR2,sig_usr)==SIG_ERR){
        err_sys("can't catch SIGUSR2");
    }
    if( (pid = fork()) <0)
        err_sys("fork error");
    else if(pid == 0){
        sleep(10);
        
    }else {
    if(waitpid(pid,NULL,0)<0)
        printf("waitpid error\n");
    }
    if(pid == 0)
        printf("child process %ld\n",(long)getpid());
    else{
        printf("parent process %ld\n",(long)getpid());
    }
    return 0;
    
    
}

static void sig_usr(int signo){
    if(signo == SIGUSR1){
        printf("received SIGUSR1\n");
        
    }else if(signo == SIGUSR2){
        printf("received SIGUSR2\n");
        
    }else{
        err_dump("received signal %d\n",signo);
    }
}