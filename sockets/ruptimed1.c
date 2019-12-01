#include "apue.h"
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFLEN 128
#define QLEN 10
#ifndef HOST_MAX_NAME
#define HOST_MAX_NAME 256
#endif

extern int initserver(int socktype, const struct sockaddr *addr, socklen_t len_addr, int len_queue);

void serve(int sockfd){
    int cli_sockfd;
    pid_t pid;

    set_cloexec(sockfd);
    for(;;){
        if( (cli_sockfd = accept(sockfd, NULL, NULL)) < 0){
            syslog(LOG_ERR, "accept error: %s", strerror(errno));
            exit(1);
        }
        if( (pid = fork()) < 0){
            syslog(LOG_ERR, "fork error: %s", strerror(errno));
            exit(1);
        }else if(pid == 0){
            if(dup2(cli_sockfd, STDOUT_FILENO) != STDOUT_FILENO || dup2(cli_sockfd, STDERR_FILENO) != STDERR_FILENO){
                syslog(LOG_ERR, "dup2 error: %s", strerror(errno));
                exit(1);
            }
            close(cli_sockfd);
            execl("/usr/bin/uptime", "uptime", (char*)0);
            syslog(LOG_ERR, "execl error: %s", strerror(errno));
            exit(1);
        }else{
            close(cli_sockfd);
            waitpid(pid, NULL, 0);
        }
    }
}
int main(int argc, char* argv[]){
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd;
    int err;
    char *hostname;
    int n;
    
    if(argc != 1){
        err_quit("usage: a.out");
    }
    if( (n = sysconf(_SC_HOST_NAME_MAX)) < 0){
        n = HOST_MAX_NAME;
    }
    if( (hostname = malloc(n)) == NULL){
        err_sys("malloc error");
    }
    if(gethostname(hostname, n) < 0){
        err_sys("gethostname error");
    }
    daemonize("uptime");
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_addr = NULL;
    hint.ai_canonname = NULL;
    hint.ai_next = NULL;
    if( (err = getaddrinfo(hostname, "uptime", &hint, &ailist)) != 0){
        syslog(LOG_ERR, "getaddrinfo error: %s", gai_strerror(err));
        exit(1);
    }
    for(aip = ailist; aip != NULL; aip = aip->ai_next){
        if( (sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0){
            serve(sockfd);
            exit(0);
        }
    }
    exit(1);
    
}