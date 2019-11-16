#include "lock_reg.h"

int lock_reg(int fd, int cmd, int type, int whence, off_t offset, off_t len){
    struct flock lock;
    lock.l_type = type;
    lock.l_whence = whence;
    lock.l_start = offset;
    lock.l_len = len;

    return fcntl(fd, cmd, &lock);
}