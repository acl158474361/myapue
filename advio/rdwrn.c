#include "rdwrn.h"

ssize_t readn(int fd, void *ptr, size_t n){
    size_t rleft = n;
    ssize_t rn;
    while(rleft > 0){
        if( (rn = read(fd, ptr, rleft)) < 0){
            if(rleft == n){
                return -1;
            }else{
                break;
            }
        }else if(rn == 0){
            break;
        }
        rleft -= rn;
        ptr += rn;

    }
    return  n-rleft;

}


size_t writen(int fd, void *ptr, size_t n){
    size_t wleft = n;
    ssize_t wn;
    while(wleft > 0){
        if( (wn = write(fd, ptr, wleft)) < 0){
            if(wleft == n){
                return  -1;
            }else{
                break;
            }
        }else if(wn == 0){
            break;
        }
        wleft -= wn;
        ptr += wn;

    }
    return n-wleft;
    
}