#include "apue.h"
#include <pthread.h>


void cleanup_routine(void *arg){
    char *str = (char*)arg;
    printf("%s\n",str);

}
void *thr_fn1(void *arg){
    printf("starting thread1\n");
    pthread_cleanup_push(cleanup_routine,"thread1 fisrt clean-up handler");
    pthread_cleanup_push(cleanup_routine,"thread1 second clean-up handler");
    if(arg){
        printf("in arg block\n");
        return ((void*)1);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return ((void*)1);
}

void *thr_fn2(void *arg){
    printf("starting thread2\n");
    pthread_cleanup_push(cleanup_routine,"thread2 first clean-up handler");
    pthread_cleanup_push(cleanup_routine,"thread2 second clean-up handler");
    if(arg){
        printf("in arg block\n");
        pthread_exit((void*)2);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void*)2);
}

int main(void){
    int err;
    pthread_t tid1,tid2;
    void *status;
    if( (err=pthread_create(&tid1,NULL,thr_fn1,(void*)1))!=0)
        err_exit(err,"can't create thread1");
    err = pthread_join(tid1,(void*)&status);
    printf("thread1 exit code is %lu\n",(unsigned long)status);
    sleep(1);
    if( (err = pthread_create(&tid2,NULL,thr_fn2,(void*)1))!=0)
        err_exit(err,"can't create thread2");
    err = pthread_join(tid2,(void*)&status);
    printf("thread2 exit code is %lu\n",(unsigned long)status);
    exit(0);
}