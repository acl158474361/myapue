#include "apue.h"
int main(){
	char buffer[4096];
	while (fgets(buffer,4096,stdin)!=NULL){
		printf("%ld\n",strlen(buffer));
		printf("first line\n");
		printf("%c空字符\n",buffer[strlen(buffer)]);
		printf("second line\n");
		printf("%c换行符\n",buffer[strlen(buffer)-1]);
		printf("last line\n");
		printf("%c最后一个\n",buffer[strlen(buffer)-2]);
		

	}
}
