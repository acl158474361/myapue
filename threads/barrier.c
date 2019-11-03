#include "apue.h"
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>


#define NTHR 8
#define NUMNUM 100L
#define TNUM (NUMNUM/NTHR)

long nums[NUMNUM];
long snums[NUMNUM];


pthread_barrier_t b;

#ifdef SOLARIS
#define heapsort qsort
#else
extern int heapsort(void *, size_t, size_t,
                    int (*)(const void *, const void *));
#endif

int complong(const void *arg1,const void *arg2){
    long l1 = *(long*)arg1;
    long l2 = *(long*)arg2;
    if(l1 == l2 )
        return 0;
    else if(l1 < l2)
        return -1;
    else 
        return 1;
}


void merge(){
    long idx[NTHR];
    long sidx,i,minidx,nextval;

    for(i = 0;i<NTHR;++i)
        idx[i] = i*TNUM;
    for(sidx = 0;sidx<NUMNUM;++sidx){
        nextval = LONG_MAX;
        for (i = 0;i<NTHR;++i){
            if( (idx[i]<(i+1)*TNUM) && (nums[idx[i]]<nextval)){
                nextval = nums[idx[i]];
                minidx = i;
            }
        }
        snums[sidx] = nextval;
        idx[minidx]++;

    }
}


void *thr_fn(void *arg){
    long idx = (long)arg;
    heapsort(&nums[idx],TNUM,sizeof(long),complong);
    pthread_barrier_wait(&b);
    return ((void*)0);
}




int main(){
    unsigned long i;
    struct timeval start,end;
    long long startusec,endusec;
    double elapsed;
    int err;
    pthread_t tid;

    srandom(1);
    for (i=0;i<NUMNUM;i++)
        nums[i] = random();
    
    gettimeofday(&start,NULL);
    pthread_barrier_init(&b,NULL,NTHR+1); //因为主线程也在等所以为NTHR+1
    for(i=0;i<NTHR;i++){
        err = pthread_create(&tid,NULL,thr_fn,(void*)(i*TNUM));
        if(err!=0)
            err_exit(err,"can't create thread");
    }
    pthread_barrier_wait(&b);
    merge();
    gettimeofday(&end,NULL);

    for (i = 0; i < NUMNUM; i++)
		printf("%ld--%ld\n",i, snums[i]);
	exit(0);

}

