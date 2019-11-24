#include "apue.h"

int main(){
    int int1, int2;
    int n;
    char line[MAXLINE];
    while( (n = read(STDIN_FILENO, line, MAXLINE)) > 0){
        //read不会为读取到的最后一个字节后面加上'\0' ,需要我们手动处理 不然数据会和前几次的数据混淆
        line[n] = 0;
        if(sscanf(line, "%d%d", &int1, &int2) == 2){//sscanf 只会扫描到第一个 '\0'
            sprintf(line, "%d\n", int1+int2); //sprintf 会在 "%d\n" 后面加上一个'\0'
            n = strlen(line); // strlen 扫描到第一个'\0' 但是返回的长度不包括 '\0'
            if(write(STDOUT_FILENO, line, n) != n){
                err_sys("write error");
            }
        }else{
            if(write(STDOUT_FILENO, "invalid args\n", 13) != 13){
                err_sys("write error");
            }
        }
        
    }
    //当父进程的写端关闭时 read会返回0
    
    err_msg("outof while block n = %d", n);
    exit(0);
}