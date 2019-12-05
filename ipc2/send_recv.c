#include "send_recv.h"
#include "apue.h"
#include <sys/socket.h>

static struct cmsghdr *send_ctlptr = NULL;
static struct cmsghdr *recv_ctlptr = NULL;
int send_err(int serv_sockfd, int status, const char *errmsg){
    int n;
    if( (n = strlen(errmsg)) > 0){
        if(write(serv_sockfd, err_msg, n) != n){
            return -1;
        }
    }
    if(status >= 0){
        status = -1;
    }

    if(send_fd(serv_sockfd, status) < 0){
        return -1;
    }
    return 0;
}

int send_fd(int serv_sockfd, int fd){
    struct msghdr msg_hdr;
    struct iovec iov[1];
    char buf[2];

    //init msghdr
    msg_hdr.msg_name = NULL;
    msg_hdr.msg_namelen = 0;
    msg_hdr.msg_iov = iov;
    msg_hdr.msg_iovlen = 1;
    iov[0].iov_base = buf;
    iov[0].iov_len = 2;
    

    if(fd < 0){
        msg_hdr.msg_control = NULL;
        msg_hdr.msg_controllen = 0;
        buf[1] = -fd;
        if(buf[1] == 0){
            buf[1] = 1;
        }
        
    }else{
        if(send_ctlptr == NULL && (send_ctlptr = malloc(CMSG_LEN(sizeof(int)))) == NULL ){
            return -1;
        }
        send_ctlptr->cmsg_level = SOL_SOCKET;
        send_ctlptr->cmsg_type = SCM_RIGHTS;
        send_ctlptr->cmsg_len = CMSG_LEN(sizeof(int));
        msg_hdr.msg_control = send_ctlptr;
        msg_hdr.msg_controllen = CMSG_LEN(sizeof(int));
        *((int*)CMSG_DATA(send_ctlptr)) = fd;
        buf[1] = 0;
    }
    buf[0] = 0;
    if(sendmsg(serv_sockfd, &msg_hdr, 0) != 2){
        return -1;
    }
    return 0;
}




int recv_fd(int cli_sockfd, ssize_t (*userfunc)(int, const void *, size_t)){
    struct msghdr msg_hdr;
    struct iovec iov[1];
    char buf[MAXLINE];


    ssize_t nr;
    char *ptr;

    int status;
    int newfd;
    
    
    for(;;){
        status = -1;
        iov[0].iov_base = buf;
        iov[0].iov_len = sizeof(buf);
        msg_hdr.msg_iov = iov;
        msg_hdr.msg_iovlen = 1;
        msg_hdr.msg_name = NULL;
        msg_hdr.msg_namelen = 0;
        
        if(recv_ctlptr == NULL && (recv_ctlptr = malloc(CMSG_LEN(sizeof(int)))) == NULL ){
            return -1;
        }
        msg_hdr.msg_control = recv_ctlptr;
        msg_hdr.msg_controllen = CMSG_LEN(sizeof(int));
        if( (nr = recvmsg(cli_sockfd, &msg_hdr, 0)) < 0){
            err_ret("recvmsg error");
            return -1;
        }
        if(nr == 0){
            err_ret("connection closed by server");
            return -1;
        }
        
        for(ptr = buf; ptr < &buf[nr];){
            if(*(ptr++) == 0){
                if(ptr != &buf[nr-1]){
                    err_ret("format error");
                    return -1;
                }
                status = *ptr & 0xFF;
                if(status == 0){
                    if(msg_hdr.msg_controllen < CMSG_LEN(sizeof(int))){
                        err_ret("status = 0, buf no fd");
                        return -1;
                    }
                    newfd = *((int*)CMSG_DATA(recv_ctlptr));
                }else{
                    newfd = -status;
                }
                nr -= 2;
            }
        }
        if(nr > 0 && userfunc(STDERR_FILENO, buf, nr) != nr){
            return -1;
        }
        if(status >= 0){
            return newfd;
        }
    }

}