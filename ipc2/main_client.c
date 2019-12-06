#include "open.h"
#include <fcntl.h>

#define BUFFSIZE 8192

int main(int argc, char* argv[]){
    char line[MAXLINE];
    char buf[BUFFSIZE];
    int fd;
    int rn;
    while(fgets(line, MAXLINE, stdin) != NULL){
        if(line[strlen(line)-1] == '\n'){
            line[strlen(line)-1] = '\0';
        }
        if( (fd = csopen(line, O_RDWR)) < 0){
            continue;
        }

        while( (rn = read(fd, buf, BUFFSIZE)) > 0){
            if(write(STDOUT_FILENO, buf, rn) != rn){
                err_sys("write error");
            }
        }
        if(rn < 0){
            err_sys("read error");
        }
        close(fd);
    }
    exit(0);
}