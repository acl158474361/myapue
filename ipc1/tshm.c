#include "apue.h"
#include <sys/shm.h>

#define ARRAYSIZE 40000
#define MALLOCSIZE 100000
#define SHMSIZE 100000
#define SHMMODE 0600

static char array[ARRAYSIZE];

int main(void){
    int shmid;
    void *addr;
    char *malloc_addr;

    printf("%14s %p to %p\n", "array from",(void*)&array[0], (void*)&array[ARRAYSIZE]);
    printf("%14s %p\n","stack around", (void*)&shmid);


    if( (malloc_addr = malloc(MALLOCSIZE)) == NULL){
        err_sys("malloc error");
    }
    printf("%14s %p to %p\n", "malloc from",(void*)malloc_addr, (void*)malloc_addr+MALLOCSIZE);

    //SHM
    if( (shmid = shmget(IPC_PRIVATE, SHMSIZE, SHMMODE)) < 0){
        err_sys("shmget error");
    }

    if( (addr = shmat(shmid, 0, 0)) == (void*)-1){
        err_sys("shmat error");
    }

    printf("%14s %p to %p\n", "shm from",(void*)addr, (void*)addr+SHMSIZE);
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}