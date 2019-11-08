#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXSTRINGSZ  4096


pthread_mutex_t envmutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
static pthread_key_t key;

extern char **environ;

static void init(void){
    pthread_key_create(&key, free);
}

char *getenv_r(const char *name){
    int len = strlen(name);
    pthread_once(&init_done, init);
    pthread_mutex_lock(&envmutex);
    char *envbuf;
    if( (envbuf = pthread_getspecific(key)) == NULL){
        envbuf = malloc(MAXSTRINGSZ*sizeof(char));
        if(envbuf == NULL){
            pthread_mutex_unlock(&envmutex);
            return NULL;
        }
        pthread_setspecific(key, envbuf);
    }

    for(int i = 0; environ[i] != NULL; ++i){
        if( (strncmp(environ[i], name, len) == 0) && (environ[i][len] == '=') ){
            strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1);
            pthread_mutex_unlock(&envmutex);
            return envbuf;
        }
    }
    pthread_mutex_unlock(&envmutex);
    return NULL;
}

