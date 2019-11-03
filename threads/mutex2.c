#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)

pthread_mutex_t hash_lock =  PTHREAD_MUTEX_INITIALIZER;

struct foo *fh[NHASH];
struct foo{
    int f_id;
    pthread_mutex_t f_lock;
    int f_count;
    struct foo *f_next;
};


struct foo *foo_alloc(int id){
    struct foo *fp;
    if( (fp = (struct foo*)malloc(sizeof(struct foo)))!=NULL){
        fp->f_id = id;
        fp ->f_count = 1;
        if( pthread_mutex_init(&fp->f_lock,NULL)!=0){
            return NULL;
            free(fp);
        }
        int idx = HASH(id);
        pthread_mutex_lock(&hash_lock);
        fp->f_next = fh[idx];
        fh[idx] = fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hash_lock);

        /* continue initialization..*/
        pthread_mutex_unlock(&fp->f_lock);

    }
    return fp;
}


void foo_hold(struct foo *fp){
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

struct foo *foo_find(int id){
    struct foo *fp;
    int idx = HASH(id);
    pthread_mutex_lock(&hash_lock);
    fp = fh[idx];
    for(;fp!=NULL;fp = fp->f_next){
        if(fp->f_id == id){
            foo_hold(fp);
            break;
        }
    }
    pthread_mutex_unlock(&hash_lock);
    return fp;

    
}


void foo_rele(struct foo *fp){
    if(!fp)
        return;
    pthread_mutex_lock(&fp->f_lock);
    if(fp->f_count == 1){
        /*这里不能用--fp->f_count == 0 
        因为如果等于0 则必须要从链表中把这个节点给删除
        需要同时获取两把锁 那么必定是先获取 hash_lock 再获取fp->f_lock
        所以先要把fp->f_lock 给unlock掉. 但是在unlock fp->f_lock时候 其他线程可以抢占
        这两个锁  这个线程就会看到一个节点的f_count为0 但是它没有被free掉，这是不符合
        程序本意的*/
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_lock(&hash_lock);
        pthread_mutex_lock(&fp->f_lock);
        if(fp->f_count !=1){
            fp->f_count--;
            pthread_mutex_unlock(&fp->f_lock);
            pthread_mutex_unlock(&hash_lock);
            return;
        }
        /*从链表中移除这个节点*/
        struct foo *pre;
        int idx = HASH(fp->f_id);
        pre = fh[idx];
        if(pre == fp){
            fh[idx] = fp->f_next;
        }else{
            while(pre->f_next !=fp){
                pre = pre->f_next;
            }
            pre->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&hash_lock);
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
        
        

    }else{
        fp->f_count--;
        pthread_mutex_unlock(&fp->f_lock);
    }
    
    

    
}


