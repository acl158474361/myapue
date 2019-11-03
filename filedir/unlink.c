#include "apue.h"
#include <fcntl.h>

int main(int argc ,char* argv[]){
	//先打开一个文件 然后解除链接 
	if(open("tempfile",O_RDWR)<0)
		err_sys("open error");
	if(unlink("tempfile")<0)
		err_sys("unlink error");
	printf("file unlinked\n");
	sleep(15);
	printf("done\n");
	exit(0);
}

