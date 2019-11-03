#include "apue.h"
#include <pthread.h>



void *thr_fn1(void *arg){
    printf("thread1 returning\n");
    return (void*)1;
}

void *thr_fn2(void *arg){
    printf("thread2 exiting\n");
    pthread_exit((void*)2);

}

int main(void){
    pthread_t tid1,tid2;
    int err;
    void *tret;

    err = pthread_create(&tid1,NULL,thr_fn1,NULL);
    if(err!=0)
        err_exit(err,"create thread1 error");
    err = pthread_create(&tid2,NULL,thr_fn2,NULL);
    if(err!=0)
        err_exit(err,"create thread2 error");
    
    err = pthread_join(tid1,&tret);
    if(err!=0)
        err_exit(err,"join thread1 error");
    printf("thread1 exit code %ld\n",(long)tret);
    
    err = pthread_join(tid2,&tret);
    if(err!=0)
        err_exit(err,"joint thread2 error");
    printf("thread2 exit code %ld\n",(long)tret);

    exit(0);
    
        
    
    






    
    
}