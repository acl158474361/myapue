#include "apue.h"
#include <sys/socket.h>

int main(){
    int nr;
    int fd[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    char *errmsg = "This is error msg";
    writen(fd[1], errmsg, strlen(errmsg));

    
    char buf[30];
    struct iovec iov[1];
    struct msghdr msg_hdr;
    msg_hdr.msg_name = NULL;
    msg_hdr.msg_namelen = 0;
    msg_hdr.msg_control = NULL;
    msg_hdr.msg_controllen = 0;
    msg_hdr.msg_iov = iov;
    msg_hdr.msg_iovlen = 1;
    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof(buf);

    if( (nr = recvmsg(fd[0], &msg_hdr, 0)) < 0){
        err_sys("recvmsg error");
    }
    buf[nr] = 0;
    printf("buf is %s\n", buf);
    
    
}