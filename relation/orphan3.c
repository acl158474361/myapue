#include "apue.h"
#include <errno.h>

static void
sig_hup(int signo)
{
	printf("SIGHUP received, pid = %ld\n", (long)getpid());
}

static void
pr_ids(char *name)
{
	printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
	    name, (long)getpid(), (long)getppid(), (long)getpgrp(),
	    (long)tcgetpgrp(STDIN_FILENO));
	fflush(stdout);
}

int
main(void)
{
	char	c;
	pid_t	pid;

	pr_ids("parent");
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid > 0) {	/* parent */
		sleep(5);	
	} else {
					/* child */
		
		pr_ids("child");
		sleep(7);
		pr_ids("child");
		if (read(STDIN_FILENO, &c, 1) != 1) //子进程被停止 ，由于产生了SIGTTIN信号 且子进程变成后台进程组，前台进程组变成SHELL
			printf("read error %d on controlling TTY\n", errno);
		
	}
	exit(0);
}
