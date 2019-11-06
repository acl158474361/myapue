#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
    int p = 0;
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_getrobust(&mattr,&p);
    printf("%d\n",p = PTHREAD_MUTEX_ROBUST);
    

}