#include "apue.h"
#include <sys/socket.h>

#define MAXSEC 128

int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t len){
    int numsec;
    for(numsec = 1; numsec <= MAXSEC; numsec <<= 1){
        if(connect(sockfd, addr, len) == 0){
            return 0;
        }

        if(numsec <= MAXSEC/2){ // MAXSEC/2 ?
            sleep(numsec);
        }
    }
    return -1;
}

