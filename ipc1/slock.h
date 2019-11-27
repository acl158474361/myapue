#include <semaphore.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>



struct slock{
    sem_t *semp;
    char name[_POSIX_NAME_MAX];
};

//创建一把锁
struct slock* s_alloc();
//销毁一把锁
void s_free(struct slock*);
//加锁
int s_lock(struct slock*);
//尝试加锁
int s_trylock(struct slock*);
//解锁
int s_unlock(struct slock*);