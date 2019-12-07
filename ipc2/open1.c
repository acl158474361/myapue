#include "open.h"
#include <sys/uio.h>


int csopen(char *name, int oflag){
    static int cli_sockfd = -1;
    char buf[10];
    struct iovec iov[3];
    ssize_t len;

    if(cli_sockfd < 0){
        if( (cli_sockfd = cli_conn(CS_OPEN)) < 0){
            err_ret("cli_conn error");
            return -1;
        }
    }
    sprintf(buf, " %d", oflag);
    iov[0].iov_base = CL_OPEN " ";
    iov[0].iov_len = strlen(CL_OPEN) + 1;
    iov[1].iov_base = name;
    iov[1].iov_len = strlen(name);
    iov[2].iov_base = buf;
    iov[2].iov_len = strlen(buf) + 1;
    len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
    if(writev(cli_sockfd, iov, 3) != len){
        err_ret("writev error");
        return -1;
    }

    return recv_fd(cli_sockfd, write);
    
}