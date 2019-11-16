#ifndef _H_LOCK_REG__
#define _H_LOCK_REG__
#include "apue.h"
#include <fcntl.h>

int lock_reg(int fd, int cmd, int type, int whence, off_t offset, off_t len);

#endif
