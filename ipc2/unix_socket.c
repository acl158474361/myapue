#include "unix_socket.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

int serv_listen(const char *name){
    struct sockaddr_un serv_addr;
    socklen_t serv_addrlen;
    int serv_sockfd;
    const int QLEN = 10;
    
    int err, rval;
    
    if(strlen(name) >= sizeof(serv_addr.sun_path)){
        errno = ENAMETOOLONG;
        return -1;
    }
    if( (serv_sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        return -2;
    }
    unlink(name);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, name);

    serv_addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if(bind(serv_sockfd, (struct sockaddr*)&serv_addr, serv_addrlen) < 0){
        rval = -3;
        goto errout;
    }
    if(listen(serv_sockfd, QLEN) < 0){
        rval = -4;
        goto errout;
        
    }

    return serv_sockfd;
errout:
    err = errno;
    close(serv_sockfd);
    errno = err;
    return rval;
}

int serv_accept(int serv_sockfd, uid_t *uidptr){
    int cli_sockfd;
    struct sockaddr_un cli_addr;
    socklen_t cli_addrlen;
    size_t cli_namelen;
    char *cli_name;
    struct stat cli_statbuf;
    time_t staletime;

    int err, rval;
    int n;
    const int STALE = 30;

    cli_addrlen = sizeof(cli_addr);
    if( (cli_sockfd = accept(serv_sockfd, (struct sockaddr*)&cli_addr, &cli_addrlen)) < 0){
        return -1;
    }

    if( (n = malloc(sizeof(cli_addr.sun_path) + 1)) == NULL){
        rval = -2;
        goto errout;
    }

    cli_namelen = cli_addrlen - offsetof(struct sockaddr_un, sun_path);
    memcpy(cli_name, cli_addr.sun_path, cli_namelen);
    cli_name[cli_namelen] = '\0';

    if(stat(cli_name, &cli_statbuf) < 0){
        rval = -3;
        goto errout;
    }

#ifdef S_ISSOCK
    if(S_ISSOCK(cli_statbuf.st_mode) == 0){
        rval = -4;
        goto errout;
    }
#endif

    if( (cli_statbuf.st_mode & (S_IRWXG | S_IRWXO)) ||
        (cli_statbuf.st_mode & S_IRWXU) != S_IRWXU){
            rval = -5;
            goto errout;
    }

    staletime = time(NULL) - STALE;
    if (cli_statbuf.st_atime < staletime ||
		cli_statbuf.st_ctime < staletime ||
		cli_statbuf.st_mtime < staletime) {
		  rval = -6;	/* i-node is too old */
		  goto errout;
	}

    if(uidptr != NULL){
        *uidptr = cli_statbuf.st_uid;
    }
    unlink(cli_name);
    free(cli_name);
    return cli_sockfd;

errout:
    err = errno;
    close(cli_sockfd);
    errno = err;
    return rval;


}

int cli_conn(const char* name){
    int cli_sockfd;
    struct sockaddr_un cli_addr;
    socklen_t cli_addrlen;
    const char *CLI_PATH = "/var/tmp";
    const int CLI_PERM = S_IRWXU;
    
    struct sockaddr_un serv_addr;
    socklen_t serv_addrlen;

    int rval, err, do_unlink = 0;

    if(strlen(name) >= sizeof(serv_addr.sun_path)){
        errno = ENAMETOOLONG;
        return -1;
    }
    if( (cli_sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        return -2;
    }
    //处理客户端地址
    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sun_family = AF_UNIX;
    sprintf(cli_addr.sun_path, "%s%05ld", CLI_PATH, (long)getpid());
    cli_addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(cli_addr.sun_path);
    unlink(cli_addr.sun_path);
    
    if(bind(cli_sockfd, (struct sockaddr*)&cli_addr, cli_addrlen) < 0){
        rval = -3;
        goto errout;
    }
    if(chmod(cli_addr.sun_path, CLI_PERM) < 0){
        rval = -4;
        do_unlink = 1;
        goto errout;
    }

    //处理 服务器地址
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, name);
    serv_addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if(connect(cli_sockfd, (struct sockaddr*)&serv_addr, serv_addrlen) < 0){
        do_unlink = 1;
        rval = -5;
        goto errout;
    }
    return cli_sockfd;

errout:
    err = errno;
    close(cli_sockfd);
    if(do_unlink){
        unlink(cli_addr.sun_path);
    }
    errno = err;
    return rval;
}



