#include "apue.h"
#include <sys/socket.h>

#define MAXSEC 128

int connect_retry(int domain, int socktype, int protocol, const struct sockaddr *addr, socklen_t len){
    int numsec;
    int sockfd;
    for(numsec = 1; numsec <= MAXSEC; numsec <<= 1){
        if( (sockfd = socket(domain, socktype, protocol)) < 0){
            return -1;
        }
        if(connect(sockfd, addr, len) == 0){

            return sockfd;
        }
        close(sockfd); //别忘了关闭文件描述符

        if(numsec <= MAXSEC/2){
            sleep(numsec);
        }
    }
    return  -1;
}