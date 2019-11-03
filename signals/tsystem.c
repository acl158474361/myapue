#include "system.h"
#include <stdio.h>

void sig_quit(int signo){
    printf("caught SIGQUIT\n");
}

void sig_chld(int signo){
    printf("caught SIGCHLD\n");
    wait(NULL);
}
int main(void){
    signal(SIGQUIT,sig_quit);
    signal(SIGCHLD,sig_chld);
    int status = system("sleep 30");
    printf("status is %d\n",status);
    printf("errno is %d\n",errno);


}