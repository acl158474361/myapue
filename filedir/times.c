#include "apue.h"
#include <fcntl.h>

int main(int argc, char* argv[]){
	//stat 结构体获取 时间信息，timespec结构体存储时间
	int i,fd;
	struct stat statbuff;
	struct timespec times[2];
	
	//循环读取文件
	for (int i=1;i<argc;i++){
		//读取status
		if(stat(argv[i],&statbuff)<0){
			err_ret("stat error");
			continue;
		}	
		if((fd=open(argv[i],O_RDWR|O_TRUNC))<0){
			err_ret("open error");
			continue;
		}
		//设置时间
		times[0] = statbuff.st_atim;
		times[1] = statbuff.st_mtim;
		if(futimens(fd,times)<0)
			err_ret("futimens error %s",argv[i]);
		close(fd); //因为有循环要关闭这个文件描述符	
	}
	exit(0);
	
}

