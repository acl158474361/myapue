#include "stdio.h"
 

int main(){
    int a = 0x04030211;
    char *p = (char*)&a;
    printf("%d\n", p[0]); //一个字节8位 2个16进制的数字 p[0] 就是16进制的最后两位 11 所以打印出来就是 17
    printf("%d\n", p[1]);
    printf("%d\n", p[2]);
    printf("%d\n", p[3]); 
}