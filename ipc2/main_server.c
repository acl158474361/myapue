#include "opend.h"
#include "send_recv.h"
char errmsg[MAXLINE];
int  oflag;
char *pathname;

int main(int argc, char* argv[]){
    int nr;
    char buf[MAXLINE];
    for(;;){
        if((nr = read(STDIN_FILENO, buf, MAXLINE)) < 0){
            err_sys("read error from stream pipe");
        }else if(nr == 0){
            break;
        }
        handle_request(buf, nr, STDOUT_FILENO);

    }
    exit(0);
}