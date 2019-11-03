#include "apue.h"
int main(void){
	char* ptr;
	size_t size;
	//先更换工作目录到指定目录
	if(chdir("/usr/lib")<0)
		err_sys("chdir failed");
	//path_alloc
	ptr = path_alloc(&size);
	if(getcwd(ptr,size)==NULL)
		err_sys("getcwd failed");
	printf("cwd is %s\n",ptr);
	exit(0);
}
