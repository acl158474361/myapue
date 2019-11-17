#include "lock_test.h"

pid_t lock_test(int fd, int type, int whence, off_t offset, off_t len){
    struct flock lock;
    lock.l_type = type;
    lock.l_whence = whence;
    lock.l_start = offset;
    lock.l_len = len;

    if(fcntl(fd, F_GETLK, &lock) < 0){
        err_sys("fcntl error");
    }
    if(lock.l_type == F_UNLCK){
        return 0;
    }

    return lock.l_pid;
}