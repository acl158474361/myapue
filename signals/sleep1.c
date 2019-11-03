#include <signal.h>
#include <unistd.h>

static void sig_alarm(int signo){

}

unsigned int sleep1(unsigned int seconds){
    if(signal(SIGALRM,sig_alarm) == SIG_ERR)
        return seconds;
    alarm(seconds);
    pause();
    return alarm(0);
}