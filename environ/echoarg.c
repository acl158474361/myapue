#include<stdio.h>

int main(int argc,char* argv[]){
	int i;
	for (i=0;argv[i]!=NULL;++i)
		printf("第 %d 个参数：%s\n",i+1,argv[i]);
	return 0;

}
