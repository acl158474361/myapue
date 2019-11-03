#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(id) (((unsigned long)id) % NHASH)

pthread_mutex_t hash_lock = PTHREAD_MUTEX_INITIALIZER;
struct foo
{
    int f_id;
    int f_count;
    pthread_mutex_t f_lock;
    struct foo *f_next;
};

struct foo *fh[NHASH];

/* using hash_lock to protect f_count and f_next while using f_lock to protect other attributes*/

struct foo *foo_alloc(int id)
{
    struct foo *fp;
    int idx;
    if ((fp = (struct foo *)malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_id = id;
        fp->f_count = 1;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);
            return NULL;
        }
        pthread_mutex_lock(&hash_lock);
        idx = HASH(fp->f_id);
        fp->f_next = fh[idx];
        fh[idx] = fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hash_lock);
        /*continue initialization....*/
        pthread_mutex_unlock(&fp->f_lock);
    }

    return fp;
}

void foo_hold(struct foo *fp)
{
    pthread_mutex_lock(&hash_lock);
    fp->f_count++;
    pthread_mutex_unlock(&hash_lock);
}

struct foo *foo_find(int id)
{
    int idx = HASH(id);
    struct foo *fp;
    pthread_mutex_lock(&hash_lock);
    fp = fh[idx];
    while (fp != NULL)
    {
        if (fp->f_id == id)
        {
            fp->f_count++;
            break;
        }
        fp = fp->f_next;
    }
    pthread_mutex_unlock(&hash_lock);
    return fp;
}

void foo_rele(struct foo *fp)
{
    pthread_mutex_lock(&hash_lock);
    if (--fp->f_count == 0)
    {
        int idx = HASH(fp->f_id);
        struct foo *pre = fh[idx];
        if (pre == fp)
        {
            fh[idx] = fp->f_next;
        }
        else
        {
            while (pre->f_next != fp)
            {
                pre = pre->f_next;
            }
            pre->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&hash_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    }
    else
    {
        pthread_mutex_unlock(&hash_lock);
    }
}
