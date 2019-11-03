#include <stdio.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
struct passwd* getpwnam(const char* name){
	struct passwd* ptr;

	setpwent();
	while( (ptr = getpwent()) !=NULL)
		if(strcmp(name,ptr->pw_name)==0)
			break;
	endpwent();
	return ptr;	




}

int main(void){
	char* name = (char*)malloc(100*sizeof(char));
	struct passwd* pwd;
	printf("输入要查询的用户名: ");
	scanf("%s",name);
	pwd = getpwnam(name);
	printf("用户的shell: %s\n",pwd->pw_shell);
	exit(0);
}
