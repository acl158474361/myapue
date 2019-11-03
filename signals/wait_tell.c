#include "wait_tell.h"
#include "signal.h"

static volatile sig_atomic_t flag = 0;
static sigset_t newset,oldset,susset;

static void sig_usr(int signo){
    flag = 1;
}
void TELL_WAIT(void){
    sigemptyset(&newset);
    sigemptyset(&susset);
    sigaddset(&newset,SIGUSR1);
    sigaddset(&newset,SIGUSR2);
    
    if(sigprocmask(SIG_BLOCK,&newset,&oldset)<0)
        err_sys("mask newset error");
    if(signal(SIGUSR2,sig_usr)==SIG_ERR)
        err_sys("can't catch SIGUSR2");
    if(signal(SIGUSR1,sig_usr)==SIG_ERR)
        err_sys("can't catch SIGUSR1");
}

void WAIT_PARENT(void){
    while(flag ==0)
        sigsuspend(&susset);
    flag = 0;
    if(sigprocmask(SIG_SETMASK,&oldset,NULL)<0)
        err_sys("reset mask error");
}

void TELL_PARENT(pid_t pid){
    kill(pid,SIGUSR1);
}

void WAIT_CHILD(void){
    while(flag == 0)
        sigsuspend(&susset);
    flag = 0;
    if(sigprocmask(SIG_SETMASK,&oldset,NULL)<0)
        err_sys("reset mask error");
}

void TELL_CHILD(pid_t pid){
    kill(pid,SIGUSR2);
}