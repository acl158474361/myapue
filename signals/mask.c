#include "apue.h"
#include "signal.h"
#include <setjmp.h>
#include <time.h>
#include "pr_mask.h"

static void sig_usr(int signo);
static void sig_alrm(int signo);
static sigjmp_buf jumbuf;
static volatile sig_atomic_t canjump;
int main(void){
    if(signal(SIGALRM,sig_alrm)==SIG_ERR)
        err_sys("can't catch SIGALRM");
    if(signal(SIGUSR1,sig_usr)==SIG_ERR)
        err_sys("can't catch SIGUSR1");
    
    pr_mask("starting main: ");
    if(sigsetjmp(jumbuf,1)){
        pr_mask("ending main: ");
        exit(0);
    }
    canjump = 1;
    for( ; ; ){
        pause();
    }
    

    
    

}

static void sig_usr(int signo){
    time_t starttime;
    if(canjump == 0)
        return;
    pr_mask("starting sig_usr: ");
    alarm(3);
    starttime = time(NULL);
    for(; ;){
        if(time(NULL)>starttime+5)
            break;
    }
    pr_mask("finishing sig_usr: ");
    canjump = 0;
    siglongjmp(jumbuf,1);


        
    
}

static void sig_alrm(int signo){
    pr_mask("in sig_alrm: ");
}