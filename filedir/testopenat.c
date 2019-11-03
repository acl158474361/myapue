#include "apue.h"
#include <fcntl.h>

int main(int argc ,char* argv[]){
	int n;
	int fd;
	char buffer[4096];
	
	if (argc!=3)
		err_quit("usage: ./a.out <path> <filename>");
	//获得相对路径的文件描述符
	if ((fd=open(argv[1],O_RDONLY))<0)
		err_ret("access path error");
	else{
	//使用openat 打开文件 获取文件的文件描述符
		if((fd=openat(fd,argv[2],O_RDWR))<0){
			err_ret("access file error");
		}else if( (n =read(fd,buffer,4096))<0){
			err_ret("read file error");
		}else if ( write(STDOUT_FILENO,buffer,n)!=n){
			err_ret("write error");
		}else
			printf("write successfully----------------\n");
			
			
	}
	exit(0);
		

}
