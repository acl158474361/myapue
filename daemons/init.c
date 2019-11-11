#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>


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

	if( (pid = fork()) < 0)
		err_quit("%s: can't fork", cmd);
	else if(pid != 0)
		exit(0);
	

	//fork 为了使进程不是 该进程所在进程组的组长进程
	setsid();
	
	

	
}