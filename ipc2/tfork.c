#include "apue.h"
int main(){
    pid_t pid;
    if( (pid = fork()) != 0){
        int i =1;
        printf("%d\n",i);
    }else{
        int i = 10;
        printf("%d\n", i);
    }
    exit(0);
}