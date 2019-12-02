#include "apue.h"
#include <sys/msg.h>

#define MAXMSZ 512

struct mymesg{
    long type;
    char text[MAXMSZ];
};

int main(int argc, char* argv[]){
    key_t key;
    struct mymesg msgent;
    int qid;
    if(argc != 3){
        err_quit("usage: a.out key msg");
    }
    key = strtol(argv[1], NULL, 0);
    memset(&msgent, 0, sizeof(msgent));

    if( (qid = msgget(key, 0)) < 0){
        err_sys("can't get msgqueue");
    }
    strncpy(msgent.text, argv[2], MAXMSZ-1);
    strcat(msgent.text, "\n");
    msgent.type = 1;
    if(msgsnd(qid, &msgent, strlen(msgent.text), 0) < 0){
        err_sys("msgsnd error");
    }
    exit(0);
}