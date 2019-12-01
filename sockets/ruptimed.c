#include "apue.h"
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <syslog.h>

#define BUFLEN 128
#define QLEN 10
#ifndef HOST_MAX_NAME
#define HOST_MAX_NAME 256
#endif

extern int initserver(int socktype, const struct sockaddr *addr, socklen_t len_addr, int len_queue);

int main(int argc, char* argv[]){
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, cli_sockfd;
    int err;
    char *hostname;
    char buf[BUFLEN];
    int n;
    FILE *fp;
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
            set_cloexec(sockfd);
            for(;;){
                if( (cli_sockfd = accept(sockfd, NULL, NULL)) < 0){
                    syslog(LOG_ERR, "connect error: %s", strerror(errno));
                    exit(1);
                }
                set_cloexec(cli_sockfd);
                if( (fp = popen("/usr/bin/uptime", "r")) == NULL){
                    sprintf(buf, "popen error: %s\n", strerror(errno));
                    send(sockfd, buf, strlen(buf), 0);
                }else{
                    while(fgets(buf, BUFLEN, fp) != NULL){
                        send(sockfd, buf, strlen(buf), 0);
                    }
                    pclose(fp);
                }
                close(cli_sockfd);
            }
        }
    }
    exit(1);
    
}