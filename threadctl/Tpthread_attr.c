#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
    int attr_robust = 0;
    int attr_type = 0;
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_getrobust(&mattr,&attr_robust);
    pthread_mutexattr_gettype(&mattr,&attr_type);
    printf("%d\n",attr_robust = PTHREAD_MUTEX_ROBUST);
    printf("%d\n",attr_type = PTHREAD_MUTEX_RECURSIVE);
    printf("%d\n",attr_type = PTHREAD_MUTEX_NORMAL);

    

}