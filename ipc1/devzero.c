#include "apue.h"
#include <sys/mman.h>
#include <fcntl.h>

#define SIZE sizeof(long)
#define NLOOP 1000

static long update(long *data){
    return (*data)++;
}

int main(int argc, char* argv[]){
    int fd;
    void *mapaddr;
    pid_t pid;
    if( (fd = open("/dev/zero", O_RDWR)) < 0){
        err_sys("open /dev/zero error");
    }

    if( (mapaddr = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
        err_sys("mmap failed");
    }

    close(fd);
    TELL_WAIT();
    if( (pid = fork()) < 0){
        err_sys("fork error");
    }else if(pid > 0){
        for(int i = 0; i < NLOOP; i+=2){
            if(i != update(mapaddr)){
                err_sys("expected %ld but got %ld", i, *((long*)mapaddr));
            }
            printf("in parent: %ld\n", *((long*)mapaddr));
            TELL_CHILD(pid);
            WAIT_CHILD();
        }


    }else{
        for(int i = 1; i < NLOOP+1; i+=2){
            WAIT_PARENT();
            if(i != update(mapaddr)){
                err_sys("expected %ld but got %ld", i, *((long*)mapaddr));
            }
            printf("in child: %ld\n", *((long*)mapaddr));
            TELL_PARENT(getppid());

        }

    }

    exit(0);
}