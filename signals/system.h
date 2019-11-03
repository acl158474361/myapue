#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int system(const char *cmdstring);