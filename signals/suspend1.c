#include "apue.h"
#include "signal.h"
#include "pr_mask.h"

static void sig_int(int signo);

int main(void ){
    sigset_t newset,oldset,waitset;
    sigemptyset(&newset);
    sigaddset(&newset,SIGINT);
    sigemptyset(&waitset);
    sigaddset(&waitset,SIGUSR1);
    pr_mask("starting main: ");
    if(signal(SIGINT,sig_int)==SIG_ERR)
        err_sys("signal SIGINT error");
    if(sigprocmask(SIG_BLOCK,&newset,&oldset) <0)
        err_sys("mask SIGINT error");
    
    pr_mask("in critical region: ");

    if(sigsuspend(&waitset) != -1)
        err_sys("sigsuspend error");
    pr_mask("after sigsuspend: ");

    if(sigprocmask(SIG_SETMASK,&oldset,NULL)<0)
        err_sys("reset mask error");
    pr_mask("ending main: ");
    exit(0);
}

void sig_int(int signo){
    pr_mask("in sig_int:");
}