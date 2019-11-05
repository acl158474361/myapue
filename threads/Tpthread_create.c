#include <pthread.h>
#include <stdio.h>

void *thr_fn(void *arg){
    long a = (long)arg;
    printf("%ld\n",a);
    
}

int main(void){
    pthread_t tid;
    pthread_create(&tid,NULL,thr_fn,(void*)10);
    pthread_join(tid,NULL);
}