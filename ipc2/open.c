#include "open.h"
#include <sys/uio.h>
#include <sys/socket.h>
#include <fcntl.h>

int csopen(char *name, int oflag){
    pid_t pid;
    struct iovec iov[3];
    int fd[2] = {-1, -1};
    char buf[10];
    if(fd[0] < 0){
        if(socketpair(AF_UNIX, SOCK_STREAM, 0, fd) < 0){
            err_ret("socketpair error");
            return -1;
        }
        if( (pid = fork()) < 0){
            err_ret("fork error");
            return -1;
        }else if(pid == 0){
            close(fd[0]);
            if(fd[1] != STDIN_FILENO && dup2(fd[1], STDIN_FILENO) != STDIN_FILENO){
                err_ret("dup2 error to stdin");
                return -1;
            }
            if(fd[1] != STDOUT_FILENO && dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO){
                err_ret("dup2 error to stdout");
                return -1;
            }
            if(execl("./main_server", "main_server", (char*)0) < 0){
                err_sys("execl error");
            }
        }
        close(fd[1]);
        
    }

    sprintf(buf, " %d", oflag);
    iov[0].iov_base = CL_OPEN " ";
    iov[0].iov_len = strlen(CL_OPEN) + 1;
    iov[1].iov_base = name;
    iov[1].iov_len = strlen(name);
    iov[2].iov_base = buf;
    iov[2].iov_len = strlen(buf) + 1;
    int len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
    if(writev(fd[0], iov, 3) != len){
        err_ret("writev error");
        return -1;
    }
        
    return recv_fd(fd[0], write);
}