#include "apue.h"

#define PAGER "${PAGER:-more}"

int main(int argc, char* argv[]){
    FILE *fp_input,*fp_pipe;

    char line[MAXLINE];

    if(argc != 2){
        err_quit("usage: a.out <inputfilepath>");
    }
    if( (fp_input = fopen(argv[1], "r")) == NULL){
        err_sys("can't open %s", argv[1]);
    }
    if( (fp_pipe = popen(PAGER, "w")) == NULL){
        err_sys("popen error");
    }

    while(fgets(line, MAXLINE, fp_input) != NULL){
        if(fputs(line, fp_pipe) == EOF){
            err_sys("fputs error to pipe");
        }
    }
    if(ferror(fp_input)){
        err_sys("fgets error");
    }
    if(pclose(fp_pipe) == -1){
        err_sys("pclose error");
    }

    exit(0);
}