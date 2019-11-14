#include "apue.h"
#include <syslog.h>
#include <string.h>
#include "single.c"




void reread(){

}
void sig_term(int signo){
    syslog(LOG_INFO, "got SIG_TERM; exiting");
    exit(0);
}

void sig_hup(int signo){
    syslog(LOG_INFO, "Re-reading configuration file");
    reread();
}

int main(int argc, char* argv[]){
    char *cmd;
    struct sigaction action_term,action_hup;

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

    action_term.sa_handler = sig_term;
    sigemptyset(&action_term.sa_mask);
    sigaddset(&action_term.sa_mask,SIGHUP);
    action_term.sa_flags = 0;

    if(sigaction(SIGTERM, &action_term, NULL) < 0){
        syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));
        exit(1);
    }

    action_hup.sa_handler = sig_hup;
    sigemptyset(&action_hup.sa_mask);
    sigaddset(&action_hup.sa_mask, SIGTERM);
    action_hup.sa_flags = 0;

    if(sigaction(SIGHUP, &action_hup, NULL) < 0){
        syslog(LOG_ERR, "can't catch SIGHUP: %s", strerror(errno));
        exit(1);
    }




    exit(0);




}