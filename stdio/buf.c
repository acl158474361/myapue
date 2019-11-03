#include "apue.h"

void pr_stdio(const char *,FILE *);
int  is_unbuffered(FILE *);
int  is_linebuffered(FILE *);
int buffer_size(FILE *);


int main(void){
	//先用标准输入标准输出标准错误 输入输出文件
	fputs("print some characters\n",stdout);
	getchar();
	fputs("print some characters to stderr\n",stderr);

	pr_stdio("stdin",stdin);
	pr_stdio("stdout",stdout);
	pr_stdio("stderr",stderr);


	//用其他文件描述符读取文件
	FILE *fp;
	fp = fopen("/etc/passwd","r");
	getc(fp);
	pr_stdio("/etc/passwd",fp);
	exit(0);

}
	

void pr_stdio(const char* name,FILE* fp){
	printf("stream = %s, ",name);
	if(is_unbuffered(fp))
		printf("unbuffered");
	else if(is_linebuffered(fp))
		printf("linebuffered");
	else 
		printf("fully buffered");
	printf(", buffer_size = %d\n",buffer_size(fp));
}
      	


#if defined(_IO_UNBUFFERED)

int
is_unbuffered(FILE *fp)
{
	return(fp->_flags & _IO_UNBUFFERED);
}

int
is_linebuffered(FILE *fp)
{
	return(fp->_flags & _IO_LINE_BUF);
}

int
buffer_size(FILE *fp)
{
	return(fp->_IO_buf_end - fp->_IO_buf_base);
}

#elif defined(__SNBF)

int
is_unbuffered(FILE *fp)
{
	return(fp->_flags & __SNBF);
}

int
is_linebuffered(FILE *fp)
{
	return(fp->_flags & __SLBF);
}

int
buffer_size(FILE *fp)
{
	return(fp->_bf._size);
}

#elif defined(_IONBF)

#ifdef _LP64
#define _flag __pad[4]
#define _ptr __pad[1]
#define _base __pad[2]
#endif

int
is_unbuffered(FILE *fp)
{
	return(fp->_flag & _IONBF);
}

int
is_linebuffered(FILE *fp)
{
	return(fp->_flag & _IOLBF);
}

int
buffer_size(FILE *fp)
{
#ifdef _LP64
	return(fp->_base - fp->_ptr);
#else
	return(BUFSIZ);	/* just a guess */
#endif
}

#else

#error unknown stdio implementation!

#endif

