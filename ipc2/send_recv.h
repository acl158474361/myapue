#ifndef _SEND_RECV__H
#define _SEND_RECV__H

#include <sys/types.h>
#include <stddef.h>

int send_fd(int serv_sockfd, int fd);

int send_err(int serv_sockfd, int status, const char *errmsg);

int recv_fd(int cli_sockfd, ssize_t (*userfunc)(int, const void *, size_t));

int send_fdu(int serv_sockfd, int fd);

int recv_fdu(int cli_sockfd, uid_t *uidptr, ssize_t (*userfunc)(int, const void *, size_t));

#endif

