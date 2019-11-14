#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

int printtag = 0;
static void
pr_ids(char *name,int tag)
{	if(tag){
        printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld sid = %ld\n",
            name, (long)getpid(), (long)getppid(), (long)getpgrp(),
            (long)tcgetpgrp(STDIN_FILENO),(long)(getsid(getpid())));
        fflush(stdout);
	}
}

void daemonize(const char *cmd){
	
	int i,fd0,fd1,fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;
	//清楚文件描述符
	umask(0);
	//获取文件描述符的最大个数 ，用来关闭所有的文件描述符
	if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
		err_quit("%s: can't get file limit", cmd);
	pr_ids("original", printtag);
	if( (pid = fork()) < 0)
		err_quit("%s: can't fork", cmd);
	else if(pid != 0)
		exit(0);
	sleep(1);
	pr_ids("first child", printtag);
	//fork 为了使进程不是 该进程所在进程组的组长进程
	setsid();
	pr_ids("first child after setsid", printtag);
	
	/*
	 * Ensure future opens won't allocate controlling TTYs.
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)  //因为posix 会向每个孤儿进程组中停止的进程发送SIGHUP 所以要捕捉它
		err_quit("%s: can't ignore SIGHUP", cmd);
	if ((pid = fork()) < 0)
		err_quit("%s: can't fork", cmd);
	else if (pid != 0) /* parent */
		exit(0);
	sleep(1);
	pr_ids("second child", printtag);

	if(chdir("/") < 0)
		err_quit("%s: can't change directory to /", cmd);

	if(rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);
	
	fd0 = open("/dev/null", O_RDWR);  //因为前面关闭了文件描述符，
									  //所以这个肯定是0
	fd1 = dup(0);
	fd2 = dup(0);


	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d",
		  fd0, fd1, fd2);
		exit(1);
	}

	
	
}

