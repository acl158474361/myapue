#include "apue.h"
#include <pthread.h>
#include <syslog.h>

sigset_t mask;

extern int already_running(void);

void reread(void){


}

void *thr_fn(void *arg){
    int err, signo;

    for(;;){
        err = sigwait(&mask, &signo);
        if(err != 0){
            syslog(LOG_ERR, "sigwait failed");
            exit(1);
        }

        switch(signo){
            case SIGHUP:
                syslog(LOG_INFO, "Re-reading configuration file");
                reread();
                break;
            case SIGTERM:
                syslog(LOG_INFO, "got SIGTERM; exiting");
                exit(0);
            
            default:
                syslog(LOG_INFO, "unexpected signal %d\n", signo);
        }
    }
    return(0);
}


int main(int argc, char* argv[]){
    int err;
    pthread_t tid;
    char *cmd;
    struct sigaction sa;

    

}