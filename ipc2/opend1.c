#include "opend.h"
#include <sys/select.h>


#define MALLOC 10

static void client_alloc(void){
    
    int i;
    if(client == NULL){
        client = malloc(MALLOC * sizeof(Client));
    }else{
        client = realloc(client, (client_size+MALLOC)*sizeof(Client));
    }
    if(client == NULL){
        err_sys("can't alloc for client array");
    }

    for(i = client_size; i < client_size + MALLOC; ++i){
        client[i].fd = -1;
    }
    client_size += MALLOC;
}

int client_add(int fd, uid_t uid){

    int i;
    if(client == NULL){
        client_alloc();
    }
again:
    for(i = 0; i < client_size; ++i){
        if(client[i].fd = -1){
            client[i].fd = fd;
            client[i].uid = uid;
            return i;
        }
    }

    client_alloc();
    goto again;
}

void client_del(int fd){
    int i;
    for(i = 0; i < client_size; ++i){
        if(client[i].fd == fd){
            client[i].fd = -1;
            return;
        }
    }
    log_quit("can't find client entry for fd %d", fd);
}


void loop_select(void){

    int listenfd; //服务器listen套接字
    int maxfd;//最大文件描述符 用于select 第一个参数
    int maxi; /* max index in client[] array */
    int n; 
    int clifd; //建立与新到来的客户端连接的socket
    int i; 
    fd_set rset, allset;
    uid_t uid; //新到来客户端的 有效进程ID


    FD_ZERO(&allset);

    if( (listenfd = serv_listen(CS_OPEN)) < 0){
        log_sys("serv_listen error");
    }
    FD_SET(listenfd, &allset);
    maxfd = listenfd;
    maxi = -1;

    for(;;){
        rset = allset;
        if( (n = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0){
            //第一次循环时 只可能是服务器的listenfd 就绪
            log_sys("select error");
        }
        if(FD_ISSET(listenfd, &rset)){
            /*新的客户端请求到来*/
            if( (clifd = serv_accept(listenfd, &uid)) < 0){
                log_sys("server_accept error: %d", clifd);
            }
            i = client_add(clifd, uid);
            FD_SET(clifd, &allset);
            if(clifd > maxfd){
                maxfd = clifd;
            }
            if(i > maxi){
                maxi = i;
            }
            log_msg("new connection: uid: %d, fd %d", uid, clifd);
            //新的客户端请求到来 加入到select的readset 中
            continue;
        }
        //不是来自新的客户端的请求，来自现有客户端的请求
        


    }

}