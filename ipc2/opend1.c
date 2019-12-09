#include "opend.h" // Related header

#include <sys/select.h> // C system headers
#include <fcntl.h>
#include <poll.h>
#include <errno.h>

#include "apue.h" // other libraries' headers

#define MALLOC 10

#define MAXARGC 50
#define WHITE " \t\n"
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
        if(client[i].fd == -1){
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
    uid_t uid; //新到来客户端的 有效用户ID
    int nr; // bytes read from client
    char buf[MAXLINE];

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
            /*新的客户端请求到来 客户端调用cli_conn时候触发*/
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
        //不是来自新的客户端的请求，来自现有客户端的请求 客户端调用cli_conn之后调用writev触发
        for(i = 0; i <= maxi; ++i){
            if( (clifd = client[i].fd) < 0){
                continue;
            }
            if(FD_ISSET(clifd, &rset)){
                if( (nr = read(clifd, buf, MAXLINE)) < 0){
                    log_sys("read error on fd %d", clifd);
                }else if(nr == 0){
                    log_msg("client closed: uid %d, fd %d", 
                        client[i].uid, clifd);
                    client_del(clifd);
                    FD_CLR(clifd, &allset);
                    close(clifd);
                }else{
                    handled_request(buf, nr, clifd, client[i].uid);
                }
            }
        }
        
    }

}

static struct pollfd* grow_pollfd(struct pollfd *pfds, int *maxfd){
    int i;
    int oldmax = *maxfd;
    int newmax = oldmax + MALLOC;
    
    if( (pfds = realloc(pfds, newmax * sizeof(struct pollfd))) == NULL){
        err_sys("realloc error");
    }
    for(i = oldmax; i < newmax; ++i){
        pfds[i].fd = -1;
        pfds[i].events = POLLIN;
        pfds[i].revents = 0;
    }
    *maxfd = newmax;
    return pfds;
}

void loop_poll(void){
    struct pollfd *pfds; // pollfd 数组
    int i;
    int listenfd; //服务器监听套接字
    int numfd = 1; //被轮询的套接字数目 最开始只有listen套接字 
    //同时也是下一个新加入客户端套接字在pfds中的下标
    int clifd;
    uid_t uid; //客户端有效用户ID
    int maxfd = MALLOC; //pfds可以容纳的最大套接字数目
    int nread; //从客户端套接字中读取的字节数
    char buf[MAXLINE];
    if( (pfds = malloc(MALLOC * sizeof(struct pollfd))) == NULL){
        err_sys("malloc error");
    }
    for(i = 0; i < MALLOC; ++i){
        pfds[i].fd = -1;
        pfds[i].events = POLLIN;
        pfds[i].revents = 0;
    }

    //把 listenfd 包含到 pfds中去
    if( (listenfd = serv_listen(CS_OPEN)) < 0){
        log_sys("serv_listen error");
    }
    client_add(listenfd, 0);
    pfds[0].fd = listenfd;

    for(;;){
        if(poll(pfds, numfd, -1) < 0){
            log_sys("poll error");
        }
        if(pfds[0].revents & POLLIN){
            /*新的客户端连接到来*/
            if( (clifd = serv_accept(listenfd, &uid)) < 0){
                log_sys("serv_accept error: %d");
            }
            client_add(clifd, uid);

            if(numfd == maxfd){
                pfds = grow_pollfd(pfds, &maxfd);
            }
            pfds[numfd].fd = clifd;
            pfds[numfd].events = POLLIN;
            pfds[numfd].revents = 0;
            numfd++;
			log_msg("new connection in poll: uid %d, fd %d", uid, clifd);
        }

        for(i = 1; i < numfd; ++i){
            if(pfds[i].revents & POLLHUP){
                goto hungup;
            }else if(pfds[i].revents & POLLIN){
                if( (nread = read(pfds[i].fd, buf, MAXLINE)) < 0){
                    log_sys("read error on fd %d", pfds[i].fd);
                }else if(nread == 0){
hungup:             
                    log_msg("closed: uid %d, fd %d",
                        client[i].uid, pfds[i].fd);
                    client_del(pfds[i].fd);
                    close(pfds[i].fd);
					if (i < (numfd-1)) {
						//如果i不是最后一个元素 压缩数组
                        pfds[i].fd = pfds[numfd-1].fd; //最后一个元素
                        pfds[i].events = pfds[numfd-1].events;
                        pfds[i].revents = pfds[numfd-1].revents;
                        i--; //i会在for循环的 最后++i语句变回原来的值
                        //所以会再次检查这个下标下的元素 也就是之前的
                        //最后一个元素
					}
					numfd--;

                }else{
                    handled_request(buf, nread, pfds[i].fd, 
                        client[i].uid);
                }
            }
        }
    }
}





void handled_request(char *buf, int nread, int clifd, uid_t uid){
    int newfd;

    if(buf[nread-1] != 0){
        snprintf(errmsg, MAXLINE-1, 
            "request from uid %d not null terminated: %*.*s\n",
            uid, nread, nread, buf);
        send_err(clifd, -1, errmsg);
        return;
    }
    log_msg("request: %s, from uid %d", buf, uid);
    
    if(buf_args(buf, cli_args) < 0){
        send_err(clifd, -1, errmsg);
        log_msg(errmsg);
        return;
    }
    if( (newfd = open(pathname, oflag)) < 0){
        snprintf(errmsg, MAXLINE-1, "can't open %s: %s\n",
            pathname, strerror(errno));
        send_err(clifd, -1, errmsg);
        log_msg(errmsg);
        return;
    }
    
    if(send_fd(clifd, newfd) < 0){
        log_sys("send_fd error");
    }
    log_msg("send fd %d over fd %d for %s", newfd, clifd, pathname);
    close(newfd);
}


int buf_args(char *buf, int (*optfunc)(int, char**)){
    char* argv[MAXARGC];
    char *ptr;
    int argc = 0;

    if(strtok(buf, WHITE) == NULL){
        return -1;
    }
    argv[argc = 0] = buf;
    while( (ptr = strtok(NULL, WHITE)) != NULL){
        if(++argc >= MAXARGC -1){
            return -1;
        }
        argv[argc] = ptr;
    }
    argv[++argc] = NULL;

    return optfunc(argc, argv);
}

int cli_args(int argc, char **argv){
    if(argc != 3 || strcmp(argv[0], CL_OPEN) != 0){
        strcpy(errmsg, "usage: <pathname> <oflag>\n");
        return -1;
    }
    pathname = argv[1];
    oflag = atoi(argv[2]);
    return 0;

}