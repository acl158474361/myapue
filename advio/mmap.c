#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>

#define COPYINCR (1024*1024*1024)

int main(int argc, char* argv[]){
    struct stat sbuf;
    int ifd, ofd;
    size_t copysz;
    off_t off = 0;
    void *des, *src;
    if(argc != 3){
        err_quit("usage: %s <inputfile> <outputfile>", argv[0]);
    }
    if( (ifd = open(argv[1], O_RDONLY)) < 0){
        err_sys("open %s error",argv[1]);
    }
    if( (ofd = open(argv[2], O_CREAT|O_RDWR|O_TRUNC, FILE_MODE)) < 0){
        err_sys("open %s error",argv[2]);
    }

    if(fstat(ifd,&sbuf) < 0){
        err_sys("fstat error");
    }
    
    if(ftruncate(ofd, sbuf.st_size) < 0){
        err_sys("truncate %s error", argv[2]);
    }

    while(off < sbuf.st_size){
        if((sbuf.st_size - off) > COPYINCR){
            copysz = COPYINCR;
        }else{
            copysz = sbuf.st_size - off;
        }

        if( (src = mmap(0, copysz, PROT_READ, MAP_SHARED, ifd, off)) == MAP_FAILED){
            err_sys("mmap inputfile error");
        }
        if( (des = mmap(0, copysz, PROT_WRITE|PROT_READ, MAP_SHARED, ofd, off)) == MAP_FAILED){
            err_sys("mmap outputfile error");
        }

        memcpy(des, src, copysz);
        munmap(src, copysz);
        munmap(des, copysz);

        off += copysz;
        
    }

    exit(0);


}