#include "apue.h"
#include <sys/wait.h>
#include <errno.h>
static void sig_user(int signo){
    printf("received signal\n");
}


int main(void){
    pid_t pid;
    
    if((pid = fork())<0)
        printf("fork error");
    else if(pid == 0){
        printf("in child\n");
        sleep(15);
    }else{
        signal(SIGUSR1,sig_user);
        while(waitpid(pid,NULL,0)<0){
            printf("waitpid error\n");
            if(errno !=EINTR){
                break;
            }else{
                printf("error EINTR\n");
            }
        }
        printf("pid is %d\n",pid);
    }
    return 0;

}