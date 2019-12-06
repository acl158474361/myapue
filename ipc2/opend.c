#include "opend.h"
#include <fcntl.h>
#include "send_recv.h"

#define MAXARGC 50
#define WHITE " \t\n"
void handle_request(char *buf, int nr, int fd){
    int newfd = -1;
    if(buf[nr-1] != '\0'){
        snprintf(errmsg, MAXLINE-1, "request not null terminated: %*.*s\n", nr, nr, buf);
        send_err(fd, -1, errmsg);
        return;
    }
    if(buf_args(buf, cli_args) < 0){
        send_err(fd, -1, errmsg);
        return;
    }
    if((newfd = open(pathname, oflag)) < 0){
        snprintf(errmsg, MAXLINE-1, "can't open %s: %s\n", pathname, strerror(errno));
        send_err(fd, -1, errmsg);
        return;
    }
    if(send_fd(fd, newfd) < 0){
        err_sys("send_fd error");
    }
    close(newfd);
}


int buf_args(char *buf, int (*optfunc)(int, char**)){
    char* argv[MAXARGC];
    char *ptr;
    int argc = 0;

    if(strtok(buf, WHITE) == NULL){
        return -1;
    }
    argv[argc = 0] = buf;
    while( (ptr = strtok(NULL, WHITE)) != NULL){
        if(++argc >= MAXARGC -1){
            return -1;
        }
        argv[argc] = ptr;
    }
    argv[++argc] = NULL;

    return optfunc(argc, argv);
}

int cli_args(int argc, char **argv){
    if(argc != 3 || strcmp(argv[0], CL_OPEN) != 0){
        strcpy(errmsg, "usage: <pathname> <oflag>\n");
        return -1;
    }
    pathname = argv[1];
    oflag = atoi(argv[2]);
    return 0;

}