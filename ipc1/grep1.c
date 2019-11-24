#include "apue.h"
#include <sys/wait.h>

int main(void){
    FILE *fp_pipe_in = popen("./myuclc", "r");
    char line[MAXLINE];
    for(;;){
        fputs("prompt> ", stdout); //由于stdout 是行缓冲 所以要flush
        fflush(stdout);

        if(fgets(line, MAXLINE, fp_pipe_in) == NULL){
            break;
        }
        if(fputs(line, stdout) == EOF){
            err_sys("output error");
        }
    }
    if(pclose(fp_pipe_in) == -1){
        err_sys("pclose error");
    }
    putchar('\n');
    exit(0);

}