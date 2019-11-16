#ifndef _H_LOCK_TEST__
#define _H_LOCK_TEST__
#include "apue.h"
#include <fcntl.h>

pid_t lock_test(int fd, int type, int whence, off_t offset, off_t len);


#endif