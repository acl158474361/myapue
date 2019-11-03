#include <shadow.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct spwd* getspnam(const char* name){
	struct spwd* ptr;
	setspent();
	while ( (ptr = getspent()) !=NULL)
		if(strcmp(name,ptr->sp_namp)==0)
			break;
	endspent();
	return ptr;
		

}

int main(void){
	char* name = (char*)malloc(100*sizeof(char));
	struct spwd* sptr;
	printf("输入要查询的用户名: ");
	scanf("%s",name);
	sptr = getspnam(name);
	printf("用户的密码是: %s\n",sptr->sp_pwdp);
	exit(0);

}
	
