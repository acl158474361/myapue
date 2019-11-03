#include<stdio.h>
#include<string.h>
#include<grp.h>
#include<stdlib.h>
struct group* getgrnam(const char* grname){
	struct group* ptr;
	setgrent();
	while( (ptr = getgrent()) !=NULL)
		if(strcmp(grname,ptr->gr_name) == 0)
			break;
	endgrent();
	return ptr;
}


int main(void){
	char* grpname = (char*)malloc(sizeof(char)*100);
	struct group* grp ;
	printf("输入要查询的组的名字： ");
	scanf("%s",grpname);
	grp = getgrnam(grpname);	
	if(grp){
		printf("组ID %d\n",grp->gr_gid);
		printf("组名 %s\n",grp->gr_name);
		char** ptr = grp->gr_mem;
		for(;*ptr!=NULL;++ptr){
			printf("用户名: %s\n",*ptr);
		}
	}
	exit(0);
}

