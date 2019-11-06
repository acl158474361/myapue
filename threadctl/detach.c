#include "apue.h"
#include <pthread.h>

int makethread(void* (*fn)(void*),void *arg){
    pthread_t tid;
    pthread_attr_t pattr;
    int err;
    if ( (err = pthread_attr_init(&pattr))!=0)
        return err;
    err = pthread_attr_setdetachstate(&pattr,PTHREAD_CREATE_DETACHED);
    if(err == 0)
        err = pthread_create(&tid,&pattr,fn,arg);
    
    pthread_attr_destroy(&pattr);
    return err;
    
}