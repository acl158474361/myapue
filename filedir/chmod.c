#include "apue.h"

int main(void){
//我们把foo文件的设置组ID打开 关闭其组执行权限
	struct stat statbuf;
	//先得到foo文件的statbuf 
	if(stat("foo",&statbuf)<0)
		err_ret("stat error for foo");
	//修改 foo文件的权限

	if(chmod("foo",(statbuf.st_mode & ~S_IXGRP) | S_ISGID))
		err_sys("chmod error for foo");
	//修改bar 文件的权限
	if(chmod("bar",(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)))
		err_sys("chmod error for bar");
	exit(0);
}
