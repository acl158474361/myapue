#include "apue.h"

static void sig_quit(int signo);

int main(void){
    sigset_t newset,oldset,pendingset;
    if(signal(SIGQUIT,sig_quit)==SIG_ERR)
        err_sys("can't catch  SIGQUIT");
    sigemptyset(&newset);
    sigaddset(&newset,SIGQUIT);
    if(sigprocmask(SIG_BLOCK,&newset,&oldset)<0)
        err_sys("SIG_BLOCK error");
    sleep(5);
    if(sigpending(&pendingset)<0)
        err_sys("sigpending error");
    if(sigismember(&pendingset,SIGQUIT))
        printf("\nSIGQUIT pending\n");
    
    if(sigprocmask(SIG_SETMASK,&oldset,NULL)<0)
        err_sys("SIG_SETMASK error");
    printf("SIGQUIT unblocked\n");
    sleep(5);
    exit(0);



}

static void sig_quit(int signo){
    printf("caught SIGQUIT\n");
    if(signal(SIGQUIT,SIG_DFL)==SIG_ERR)
        err_sys("can't reset SIGQUIT");
}