#include "apue.h"
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char* argv[]){
    int sockfd;
    struct sockaddr_un addr;
    int size;
    if( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        err_sys("socket error");
    }
    strcpy(addr.sun_path, "foo.socket");
    addr.sun_family = AF_UNIX;
    size = offsetof(struct sockaddr_un, sun_path) + strlen(addr.sun_path);
    if(bind(sockfd, (struct sockaddr*)&addr, size) < 0){
        err_sys("bind error");
    }
    printf("UNIX domain socket bound\n");
    exit(0);
}