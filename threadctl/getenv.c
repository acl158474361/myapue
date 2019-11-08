#include <limits.h>
#include <string.h>

#define MAXSTRINGSZ 4096

static char envbuf[MAXSTRINGSZ];

extern char **environ;

char *getenv(const char *name){
    int len = strlen(name);
    for (int i = 0; environ[i] != NULL; ++i){
        if( (strncmp(environ[i], name, len)) && (environ[i][len] == '=') ){
            strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1);
            return envbuf;
        }
    }
}

