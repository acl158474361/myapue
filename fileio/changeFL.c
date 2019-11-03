#include "apue.h"
#include <fcntl.h>

void set_fl(int fd,int flags){
	int val;
	//先获取 文件状态标志
	if((val=fcntl(fd,F_GETFL,0))<0)
		err_sys("get F_GETFL error");
	//turn on flags
	val |= flags;
	//设置文件状态标志
	if(fcntl(fd,F_SETFL,val)<0)
		err_sys("set F_SETFL error");
		
}
