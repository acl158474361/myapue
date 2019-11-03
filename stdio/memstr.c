#include "apue.h"
#define BSZ 48

int main(void){
	char buf[BSZ];
	FILE *fp;
	memset(buf,'a',BSZ-2);
	buf[BSZ-2] = '\0';
	buf[BSZ-1] = 'X';
	printf("%s\n",buf);
	printf("len of string in buffer = %ld\n",(long)strlen(buf));	
	if ( (fp = fmemopen(buf,BSZ,"w+")) == NULL)
		err_sys("fmemopen error");
	printf("initial buffer contents: %s\n",buf);
	fprintf(fp,"hello, world");
	printf("before flush: %s\n",buf);
	fflush(fp);
	printf("after flush: %s\n",buf);
	printf("len of string in buffer = %ld\n",(long)strlen(buf));	
	
	printf("after flush buf+12: %s\n",buf+12);

	printf("after flush buf+13: %s\n",buf+13);
	printf("len of string in buffer = %ld\n",(long)strlen(buf+13));	
	memset(buf,'b',BSZ-2);
	buf[BSZ-2] = '\0';
	buf[BSZ-1] = 'X';
	fseek(fp,1,SEEK_SET); //只要SEEK_SET的值为0 就不会在写入hello world 之后再追加null字节
	fprintf(fp,"hello, world");
	fseek(fp,0,SEEK_SET);
	printf("after fseek: %s\n",buf);

	printf("len of string in buffer = %ld\n",(long)strlen(buf));	
	printf("after fseek: buf+13 %s\n",buf+13);
	
	printf("after fseek: buf+14 %s\n",buf+14);
	printf("len of string in buffer = %ld\n",(long)strlen(buf));	
	
	memset(buf,'c',BSZ-2);
	
	buf[BSZ-2] = '\0';
	buf[BSZ-1] = 'X';

	fprintf(fp,"hello, world");
	fclose(fp);
	printf ("after close: %s\n",buf);
	
	printf("len of string in buffer = %ld\n",(long)strlen(buf));	
	exit(0);
}

