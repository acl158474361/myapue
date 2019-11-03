#include "apue.h"
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void){
	//创建新文件
	int fd;
	if ((fd=creat("file_hole",FILE_MODE))<0)
		err_sys("creat error");
	//写入数据
	if (write(fd,buf1,10)!=10)
		err_sys("write error");
	//设置偏移量
	if (lseek(fd,16384,SEEK_SET)==-1)
		err_sys("lseek error");
	
	if (write(fd,buf2,10)!=10)
		err_sys("write error");
	exit(0);
}
