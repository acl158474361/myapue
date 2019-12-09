#ifndef _UNIX_SOCKET__H
#define _UNIX_SOCKET__H

#include <sys/types.h>

int serv_listen(const char *name);

int serv_accept(int serv_sockfd, uid_t *uidptr);

int cli_conn(const char *name);

#endif