#include "pr_mask.h"

void pr_mask(const char *str){
    //先得到旧的sigset_t
    sigset_t s;
    int errnosave;
    errnosave = errno;
    if(sigprocmask(0,NULL,&s)<0)
        err_ret("sigprocmask error");
    else{
        printf("%s",str);
        if(sigismember(&s,SIGINT))
            printf(" SIGINT");
        if(sigismember(&s,SIGQUIT))
            printf(" SIGQUIT");
        if(sigismember(&s,SIGUSR1))
            printf(" SIGUSR1");
        if(sigismember(&s,SIGUSR2))
            printf(" SIGUSR2");
        if(sigismember(&s,SIGALRM))
            printf(" SIGALRM");
        printf("\n");
    }
    errno = errnosave;
    
}