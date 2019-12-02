#include "apue.h"
#include <poll.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/socket.h>

#define QSIZE 3
#define MAXMSZ 512
#define KEY 0x123
struct msg_info{
    int msg_id;
    pthread_t t_id;
    int fds[2];
};

struct mymesg{
    long mtype;
    char mtext[MAXMSZ];
};

void *thr_fn(void *arg){
    
    struct msg_info *info = (struct msg_info*)arg;
    int msg_id = info->msg_id;
    struct mymesg msgent;
    int n;
    //从消息队列中获取数据 并且写入到unix域套接字中
    for(;;){
        if( (n = msgrcv(msg_id, &msgent, MAXMSZ, 0, MSG_NOERROR)) < 0){
            err_msg("msgrcv error, msg_id = %ld, t_id = %ld", info->msg_id, info->t_id);
            pthread_exit((void*)-1);
        }
        //写入数据
        msgent.mtext[n] = '\0';
        if(write(info->fds[1], (&msgent)->mtext, strlen((&msgent)->mtext)) != strlen(msgent.mtext)){
            err_msg("write error, msg_id = %ld, t_id = %ld", info->msg_id, info->t_id);
            pthread_exit((void*)-1);
        }
    }
}

int main(int argc, char* argv[]){
    struct msg_info msgs[QSIZE];
    struct pollfd pollfds[QSIZE];
    int err;
    int n;
    char buf[MAXMSZ];

    
    for(int i = 0; i < QSIZE; ++i){
        if( (msgs[i].msg_id = msgget((KEY+i), IPC_CREAT|0666)) < 0){
            err_sys("create msgq error");
        }
        printf("queue ID %d is %d\n", i, msgs[i].msg_id);
        if(socketpair(AF_UNIX, SOCK_DGRAM, 0, msgs[i].fds) < 0){
            err_sys("socketpair error");
        }
        pollfds[i].fd = msgs[i].fds[0];
        pollfds[i].events = POLLIN;
        if( (err = pthread_create(&msgs[i].t_id, NULL, thr_fn, &msgs[i])) != 0){
            err_exit(err, "pthread_create error");
        }
    }

    for(;;){
        if(poll(pollfds, QSIZE, -1) < 0){
            err_sys("poll error");
        }
        for(int i = 0; i < QSIZE; ++i){
            if(pollfds[i].revents & POLLIN){
                while( (n = read(pollfds[i].fd, buf, MAXMSZ)) > 0){
                    write(STDOUT_FILENO, buf, n);
                }
                if(n < 0){
                    err_sys("read from %d error", pollfds[i].fd);
                }
            }
        }

    }
    exit(0);

}