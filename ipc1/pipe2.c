#include "apue.h"
#include <sys/wait.h>

#define DEF_PAGER "/bin/more"

int main(int argc, char *argv[]){
    int fd[2]; //pipe文件描述符
    FILE *inputFP; //输入文件流
    char *argv0; //execl argv0参数
    pid_t pid; //子进程pid
    char line[MAXLINE]; //fgets 缓冲区
    size_t n;
    char *pager;


    if(argc != 2){
        err_quit("usage: a.out <inputfilepath>");
    }
    if( (inputFP = fopen(argv[1], "r")) == NULL){
        err_sys("can't open %s", argv[1]);
    }
    if(pipe(fd) < 0){
        err_sys("pipe error");
    }

    if( (pid = fork()) < 0){
        err_sys("fork error");
    }else if(pid > 0){
        //parent
        close(fd[0]);

        while(fgets(line, MAXLINE, inputFP) != NULL){
            n = strlen(line);
            if(write(fd[1], line, n) != n){
                err_sys("write error to pipe");
            }
        }

        if(ferror(inputFP)){
            err_sys("fgets error");
        }
        close(fd[1]);

        if(waitpid(pid, NULL, 0) < 0){
            err_sys("waitpid error");
        }
    }else{
        close(fd[1]);

        if(fd[0] != STDIN_FILENO){
            if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){
                err_sys("dup2 failed");
            }
            close(fd[0]);
        }
        
        if( (pager = getenv("PAGER")) == NULL){
            pager = DEF_PAGER;
        }
        if( (argv0 = strrchr(pager, '/')) != NULL){
            argv0++;
        }else{
            argv0 = pager;
        }

        if(execl(pager, argv0, (char*)0) < 0){
            err_sys("execl %s error", pager);
        }

    }

    exit(0);


}