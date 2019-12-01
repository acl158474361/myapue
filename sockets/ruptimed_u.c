#include "apue.h"
#include <netdb.h>
#include <sys/socket.h>
#include <syslog.h>
#include <errno.h>

#define BUFLEN 128
#define MAXADDRLEN 256
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

extern int initserver(int socktype, const struct sockaddr *addr, socklen_t alen, int qlen);

void serve(int sockfd){
    //服务器接受客户端请求，发送数据
    struct sockaddr *addr;
    char addr_buf[MAXADDRLEN];
    addr = (struct sockaddr *)addr_buf;
    socklen_t alen;

    int n;
    char buf[BUFLEN];

    FILE *fp;

    set_cloexec(sockfd);
    for(;;){
        alen = MAXADDRLEN;
        if( (n = recvfrom(sockfd, buf, BUFLEN, 0, addr, &alen)) < 0){
            syslog(LOG_ERR, "recvfrom error: %s", strerror(errno));
            exit(1);
        }
        //可以开始发送数据
        //用管道来获取数据
        if( (fp = popen("/usr/bin/uptime", "r")) == NULL){
            sprintf(buf, "popen error: %s\n", strerror(errno));
            sendto(sockfd, buf, strlen(buf), 0, addr, alen);
        }else{
            while(fgets(buf, BUFLEN, fp) != NULL){
                sendto(sockfd, buf, strlen(buf), 0, addr, alen);
            }
            pclose(fp);
        }
    }


}
int main(int argc, char* argv[]){
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, err, n;
    char *hostname;

    if(argc != 1){
        err_quit("usage: a.out");
    }
    if( (n = sysconf(_SC_HOST_NAME_MAX)) < 0){
        n = HOST_NAME_MAX;
    }
    if( (hostname = malloc(n)) == NULL){
        err_sys("malloc error");
    }
    if(gethostname(hostname, n) < 0){
        err_sys("gethostname error");
    }
    daemonize("uptime");
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_socktype = SOCK_DGRAM;
    hint.ai_flags = AI_CANONNAME;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    if( (err = getaddrinfo(hostname, "uptime", &hint, &ailist)) != 0){
        syslog(LOG_ERR, "getaddrinfo error: %s", gai_strerror(err));
        exit(1);
    }
    for(aip = ailist; aip != NULL; aip = aip->ai_next){
        if( (sockfd = initserver(SOCK_DGRAM, aip->ai_addr, aip->ai_addrlen, 0)) >= 0){
            serve(sockfd);
            exit(0);
        }
    }
    exit(1);

}