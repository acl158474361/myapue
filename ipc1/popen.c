#include "apue.h"
#include <limits.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#define OPEN_MAX_GUESS	256

static pid_t *child_pids = NULL;

static int maxfd;

long
open_max(void)
{
	long openmax;
	struct rlimit rl;

	if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 ||
	  openmax == LONG_MAX) {
		if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
			err_sys("can't get file limit");
		if (rl.rlim_max == RLIM_INFINITY)
			openmax = OPEN_MAX_GUESS;
		else
			openmax = rl.rlim_max;
	}
	return(openmax);
}

FILE *popen(const char *cmd, const char *type){
    FILE *fp;
    int pfd[2];
    pid_t pid;

    if((type[0] != 'r' && type[0] != 'w') || type[1] != 0){
        errno = EINVAL;
        return NULL;
    }

    if(child_pids == NULL){
        maxfd = open_max();
        if( (child_pids = calloc(maxfd, sizeof(pid_t))) == NULL){
            return NULL;
        }
    }

    if(pipe(pfd) < 0){
        return NULL;
    }

    if(pfd[0] >= maxfd || pfd[1] >= maxfd){
        close(pfd[0]);
        close(pfd[1]);
        errno = EMFILE;
        return NULL;
    }

    if( (pid = fork()) < 0){
        return NULL;
    }else if(pid == 0){
        //in child
        if(type[0] == 'r'){
            close(pfd[0]);
            if(pfd[1] != STDOUT_FILENO){
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }
        }else{
            close(pfd[1]);
            if(pfd[0] != STDIN_FILENO){
                dup2(pfd[0], STDIN_FILENO);
                close(pfd[0]);
            }

        }

        for(int i = 0; i < maxfd; ++i){
            if(child_pids[i] > 0){
                close(i);
            }
        }

        execl("/bin/sh", "sh", "-c", cmd, (char*)0);
        _exit(127);
    }

    if(type[0] == 'r'){
        close(pfd[1]);
        if( (fp = fdopen(pfd[0], type)) == NULL){
            return NULL;
        }

    }else{
        close(pfd[0]);
        if( (fp = fdopen(pfd[1], type)) == NULL){
            return NULL;
        }

    }

    child_pids[fileno(fp)] = pid;
    return fp;


}


int pclose(FILE *fp){
    int stat; //返回状态
    pid_t pid;
    int fd;
    if(child_pids == NULL){
        errno = EINVAL;
        return -1;
    }
    if( (fd = fileno(fp)) >= maxfd){
        errno = EINVAL;
        return -1;
    }

    if( (pid = child_pids[fd]) == 0){
        errno = EINVAL;
        return -1;
    }

    child_pids[fd] = 0;
    if(fclose(fp) == EOF){
        return -1;
    }

    while(waitpid(pid, &stat, 0) < 0){
        if(errno != EINTR){
            return -1;
        }
    }

    return stat;

}