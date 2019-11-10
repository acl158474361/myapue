#include "apue.h"
#include <pthread.h>

static pthread_mutex_t mutex_flag = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_flag = PTHREAD_COND_INITIALIZER;
static int flag = 0;
static sigset_t sigset_mask;

void *thr_fn(void *arg){
    int err,signo;
    for(;;){
        err = sigwait(&sigset_mask,&signo);
        if(err !=0)
            err_exit(err,"sigwait error");

        switch(signo){
            case SIGINT:
                printf("\ninterrupt\n");
                break;
            case SIGQUIT:
                pthread_mutex_lock(&mutex_flag);
                flag = 1;
                pthread_mutex_unlock(&mutex_flag);
                pthread_cond_signal(&cond_flag);
                return ((void*)0);
            default:
                printf("unexpected signal %d\n",signo);
                exit(1);
        }
    }


    

}

int main(void){
    sigset_t sigset_saved;
    sigemptyset(&sigset_mask);
    sigaddset(&sigset_mask,SIGINT);
    sigaddset(&sigset_mask,SIGQUIT);
    pthread_t tid;
    int err;

    if( (err = pthread_sigmask(SIG_BLOCK,&sigset_mask,&sigset_saved)) != 0)
        err_exit(err,"sigpromask error");
    if( (err = pthread_create(&tid,NULL,thr_fn,NULL)) != 0)
        err_exit(err,"pthread_create error");

    

    pthread_mutex_lock(&mutex_flag);

    while(flag == 0)
        pthread_cond_wait(&cond_flag,&mutex_flag);
    

    pthread_mutex_unlock(&mutex_flag);

    flag = 0;
    if(sigprocmask(SIG_SETMASK,&sigset_saved,NULL) < 0)
        err_sys("reset mask error");

    exit(0);
}