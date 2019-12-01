#include "apue.h"
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#define BUFLEN 128

extern int connect_retry(int domain, int socktype, int protocol, const struct sockaddr *addr, socklen_t len_addr);

int main(int argc, char* argv[]){
    struct addrinfo *ailist, *aip;
    struct addrinfo hint; //用于getaddrinfo
    int sockfd; 
    int err;
    int n;
    char buf[BUFLEN];
    
    if(argc != 2){
        err_quit("usage: a.out <hostname>");
    }
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_addr = NULL;
    hint.ai_canonname = NULL;
    hint.ai_next = NULL;

    if( (err = getaddrinfo(argv[1], "uptime", &hint, &ailist)) != 0){
        err_quit("getaddrinfo error: %s", gai_strerror(err));
    }
    for(aip = ailist; aip != NULL; aip = aip->ai_next){
        if( (sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0,
            aip->ai_addr, aip->ai_addrlen)) >= 0){
            while( (n = recv(sockfd, buf, BUFLEN, 0)) > 0){
                write(STDOUT_FILENO, buf, n);
            }
            if(n < 0){
                err_sys("recv error");
            }
            exit(0);
        }else{
            err = errno;
        }
    }
    err_exit(err, "can't connect to %s", argv[1]);
    
}