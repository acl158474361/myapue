#include "apue.h"

static void sig_pipe(int signo);

int main(void){
    pid_t pid;
    int pfd1[2], pfd2[2];
    char line[MAXLINE];
    int n;

    if(signal(SIGPIPE, sig_pipe) < 0){
        err_sys("signal error");
    }

    if(pipe(pfd1) < 0 || pipe(pfd2) < 0){
        err_sys("pipe error");
    }

    if( (pid = fork()) < 0){
        err_sys("fork error");
    }else if(pid > 0){
        //parent

        close(pfd1[0]);
        close(pfd2[1]);

        while(fgets(line, MAXLINE, stdin) != NULL){
            n = strlen(line); // fgets 会在最后加上一个'\0'
            if(write(pfd1[1], line, n) != n){
                err_sys("write to child error");
            }
            if( (n = read(pfd2[0], line, MAXLINE)) < 0){
                err_sys("read from child error");
            }
            if(n == 0){
                err_msg("child has closed stdout");
                break;
            }
            line[n] = '\0'; //read读到的数据不是以 '\0' 结尾 我们需要处理它以便用 fputs输出
            if(fputs(line, stdout) == EOF){
                err_sys("fputs error");
            } 
        }
        if(ferror(stdin)){
            err_sys("fgets error");
        }
        
        exit(0);  //exit 时候 所有文件描述符关闭 管道的写端 pfd1[1] 关闭

    }else{
        //child
        close(pfd1[1]);
        close(pfd2[0]);
        if(pfd1[0] != STDIN_FILENO){
            dup2(pfd1[0], STDIN_FILENO);
            close(pfd1[0]);
        }

        if(pfd2[1] != STDOUT_FILENO){
            dup2(pfd2[1], STDOUT_FILENO);
            close(pfd2[1]);
        }

        if(execl("./add", "add", (char*)0) < 0){
            err_sys("execl error");
        }
    }
    exit(0);
}

static void sig_pipe(int signo){
    printf("SIGPIPE caught\n");
    exit(1);
}