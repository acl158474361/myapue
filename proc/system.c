#include "system.h"
int system(const char* cmdstring){
    int status;
    pid_t pid;
    if((pid = fork())<0)
        status = -1;
    else if(pid == 0){
        execl("/bin/sh","sh","-c",cmdstring,(char*)0);
        _exit(127);   //execl 失败才返回
    }else{
        //parent
        while( waitpid(pid,&status,0)<0){
            if(errno!=EINTR){
                status = -1;
                break;
            }
        }
        
    }
    return status;

}
