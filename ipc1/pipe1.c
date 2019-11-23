#include "apue.h"


int main(void){
    int fd[2];
    char line[MAXLINE];
    pid_t pid;
    ssize_t n;

    if(pipe(fd) < 0){
        err_sys("pipe error");
    }
    if( (pid = fork()) < 0){
        err_sys("fork error");
    }else if(pid > 0){
        //parent
        close(fd[0]);
        write(fd[1], "hello, world!\n", 14);
    }else{
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    exit(0);
    


}