#include "apue.h"
#include <sys/socket.h>
#include <errno.h>

int initserver(int socktype, const struct sockaddr *addr, socklen_t alen, int qlen){
    int sockfd;
    int reuse = 1;
    int err;
    if( (sockfd = socket(addr->sa_family, socktype, 0)) < 0){
        return -1;
    }
    if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
        goto errout;
    }
    if( bind(sockfd, addr, alen) < 0){
        goto errout;
    }
    if(socktype = SOCK_STREAM || socktype == SOCK_SEQPACKET){
        if(listen(sockfd, qlen) < 0){
            goto errout;
        }
    }
    return sockfd;


    errout:
        err = errno;
        close(sockfd);
        errno = err;
        return -1;
}