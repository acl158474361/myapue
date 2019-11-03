#include "apue.h"
#include "signal.h"
#include "pr_mask.h"


volatile sig_atomic_t flag = 0;
static void sig_quit(int signo){
    if(signo == SIGINT){
        printf("\ninterrupted\n");
        pr_mask("in SIGINT: ");
    }
    if(signo ==SIGQUIT){
        flag =1;
        pr_mask("in SIGQUIT: ");
    }
}

int main(void){
    sigset_t newset,oldset,susset;
    if(signal(SIGINT,sig_quit)==SIG_ERR)
        err_sys("catch SIGINT error");
    if(signal(SIGQUIT,sig_quit)==SIG_ERR)
        err_sys("catch SIGQUIT error");
    
    sigemptyset(&newset);
    sigemptyset(&susset);
    sigaddset(&newset,SIGQUIT);
    if(sigprocmask(SIG_BLOCK,&newset,&oldset)<0)
        err_sys("mask SIGQUIT error");
    pr_mask("before suspend: ");
    while(flag == 0)
        sigsuspend(&susset);
    pr_mask("after suspend: ");

    if(sigprocmask(SIG_SETMASK,&oldset,NULL)<0)
        err_sys("reset mask error");
    pr_mask("before exit: ");
    exit(0);
    
    
}