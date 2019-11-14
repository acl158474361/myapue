#include "apue.h"
#include <pthread.h>
#include <syslog.h>
#include <string.h>

static sigset_t mask;
extern int already_running(void);

void reread(void){

}

void *thr_fn(void *arg){
    int err,signo;
    for(;;){
        err = sigwait(&mask,&signo);
        if(err != 0){
            syslog(LOG_ERR, "sigwait failed");
            exit(1);
        }
        switch (signo)
        {
        case SIGHUP:
            syslog(LOG_INFO, "Re-reading cofiguration file");
            reread();
            break;
        case SIGTERM:
            syslog(LOG_INFO, "got SIGTERM; exiting");
            exit(0);
        default:
            syslog(LOG_INFO, "got unexpected signal %d",signo);
            
        }
    }
    return(0);
}


int main(int argc, char* argv[]){
    int err;
    char *cmd;
    pthread_t tid;
    struct sigaction sa;
    if( (cmd = strrchr(argv[0], '/')) != NULL){
        cmd++;
    }else{
        cmd = argv[0];
    }

    daemonize(cmd);

    if(already_running()){
        syslog(LOG_ERR, "daemon already running");
        exit(1);
    }

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    //恢复 daemonize 中的SIGHUP action

    if(sigaction(SIGHUP, &sa, NULL) < 0){
        err_quit("%s: can't restore SIGHUP default");
    }

    sigfillset(&mask);
    if( (err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0){
        err_exit(err, "SIG_BLOCK error");
    }

    if( (err = pthread_create(&tid, NULL, thr_fn, 0)) != 0){
        err_exit(err, "can't create thread");
    }

    exit(0);


}

