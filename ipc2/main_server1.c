#include <syslog.h> // C system headers
#include <getopt.h>

#include "apue.h" // other libraries' headers

#include "opend.h" // your project's headers

int debug, oflag, client_size, log_to_stderr;

char errmsg[MAXLINE];
char *pathname;
Client *client = NULL;


int main(int argc, char* argv[]){
    int c;
    log_open("open.serv", LOG_PID, LOG_USER);

    opterr = 0;
    while( (c = getopt(argc, argv, "d")) != EOF){
        switch (c)
        {
        case 'd': /*debug*/
            debug = log_to_stderr = 1;
            break;
        
        case '?':
            err_quit("unrecognized option: -%c", optopt);
        }
    }

    if(debug == 0){
        daemonize("opend"); //如果不debug 就使用守护进程 deamonize改变了工作目录
    }
    loop_poll();
}