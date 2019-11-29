#include "apue.h"
#include <sys/socket.h>
#include <errno.h>

int initserver(int type, const struct sockaddr *addr, socklen_t len_addr, int len_queue){
    int err;
    int sockfd;
    if( (sockfd = socket(addr->sa_family, type, 0)) < 0){
        return -1;
    }
    if(bind(sockfd, addr, len_addr) < 0){
        goto errout;
    }
    if(type == SOCK_STREAM || type == SOCK_SEQPACKET){
        //面向连接
        if(listen(sockfd, len_queue) < 0){
            goto errout;
        }
    }
    return sockfd;

errout:
/*为了使 errno 不会被close() 函数覆盖*/
    err = errno;
    close(sockfd);
    errno = err;
    return -1;
}