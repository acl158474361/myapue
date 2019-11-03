#ifndef _SYS_DEM
#define _SYS_DEM

#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int system(const char* cmdstring);

#endif
