#include "apue.h"
int main(int argc,char* argv[]){
	//三个参数
	int i ; //参数数量 用于for循环
	struct stat buf; //存储文件详细信息的结构体
	char *ptr ; //存储打印信息字符串

	for (i=1;i<argc;i++){
		printf("%s: ",argv[i]);
		//获取文件信息
		if(lstat(argv[i],&buf)<0){
			err_ret("lstat error");
			continue;
		}
				
		if (S_ISREG(buf.st_mode))
			ptr = "regular file";
		else if(S_ISDIR(buf.st_mode))
			ptr = "directory";
		else if(S_ISCHR(buf.st_mode))
			ptr = "character special";
		else if(S_ISBLK(buf.st_mode))
			ptr = "block special";
		else if(S_ISFIFO(buf.st_mode))
			ptr = "fifo";
		else if(S_ISLNK(buf.st_mode))
			ptr = "symbolic link";
		else if(S_ISSOCK(buf.st_mode))
			ptr = "socket";
		else
			ptr = "** unknown mode **";
		printf ("%s\n",ptr);

		
	}
	exit(0);
}
