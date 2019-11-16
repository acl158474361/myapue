#include "apue.h"
#include <fcntl.h>
#include <errno.h>

static void lockbyte(int fd, off_t offset, const char *name){
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = 1;
    if(fcntl(fd, F_SETLKW, &lock) < 0)
        err_sys("%s: lockbyte error",name);
}
static void lockabyte(int fd, off_t offset, const char *name){
    if(writew_lock(fd, offset, SEEK_SET, 1) < 0)
        err_sys("%s: lockabyte error", name);
}

int main(void){
    
    int fd = open("tempfile", O_CREAT|O_RDWR|O_TRUNC, FILE_MODE);
    dprintf(fd, "ab");

    pid_t pid;
    TELL_WAIT();
    if( (pid = fork()) < 0){
        err_exit(errno, "fork error");
    }else if(pid == 0){
        lockabyte(fd,0,"child");
        WAIT_PARENT();
        TELL_PARENT(getppid());
        lockabyte(fd,1,"child");
    }else{
        lockabyte(fd,1,"parent");
        TELL_CHILD(pid);
        WAIT_CHILD();
        lockabyte(fd,0,"parent");
    }

    exit(0);
    

}