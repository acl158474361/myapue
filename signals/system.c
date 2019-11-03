#include "system.h"


int system(const char* cmdstring){
    int status; //system返回状态码
    pid_t pid;
    sigset_t mask_child,mask_saved; 
    struct sigaction action_savedquit,action_savedint,action_ign;

    
    action_ign.sa_handler = SIG_IGN;
    sigemptyset(&action_ign.sa_mask);
    action_ign.sa_flags = 0;
    sigemptyset(&mask_child);
    sigaddset(&mask_child,SIGCHLD);
    
    if(sigaction(SIGINT,&action_ign,&action_savedint)<0)
        return -1;
    if(sigaction(SIGQUIT,&action_ign,&action_savedquit)<0)
        return -1;
    
    if(sigprocmask(SIG_BLOCK,&mask_child,&mask_saved)<0)
        return -1;
    
    if((pid=fork())<0)
        status = -1;
    else if(pid == 0){
        //恢复子进程的限制
        sigaction(SIGINT,&action_savedint,NULL);
        sigaction(SIGQUIT,&action_savedquit,NULL);
        sigprocmask(SIG_SETMASK,&mask_saved,NULL);
        execl("/bin/sh","sh","-c",cmdstring,(char*)0);
        _exit(127);
    }else{
        while(waitpid(pid,&status,0)<0){
            if(errno!=EINTR){
                status = -1;
                break;
            }
        }
        

    }
    if(sigaction(SIGINT,&action_savedint,NULL)<0)
        return -1;
    if(sigaction(SIGQUIT,&action_savedquit,NULL)<0)
        return -1;

    if(sigprocmask(SIG_SETMASK,&mask_saved,NULL)<0)
        return -1;

    
    return status;

}