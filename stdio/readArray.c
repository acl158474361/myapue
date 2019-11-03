#include "apue.h"
#include <stdio.h>
int main(){
	int data[10] ={1,2,3,4,5,6,7,8,9,10}; 
	FILE *fp =  fopen("/home/icecream/floatdata","w");
	if(fwrite(&data[2],sizeof(int),4,fp)!=4)
		err_sys("fwrite error");	
	
}
