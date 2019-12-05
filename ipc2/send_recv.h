#ifndef _SEND_RECV__
#define _SEND_RECV__

#include "apue.h"
int send_fd(int serv_sockfd, int fd);

int send_err(int serv_sockfd, int status, const char *errmsg);

int recv_fd(int cli_sockfd, ssize_t (*userfunc)(int, const void *, size_t));

#endif

