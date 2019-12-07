#include "apue.h"
#include <errno.h>
#include "unix_socket.h"

#define CL_OPEN "open"
#define CS_OPEN "/tmp/opend.socket"

extern char errmsg[];
extern int oflag;
extern char *pathname;
extern int debug;

typedef struct{
    int fd;
    uid_t uid;
} Client;

extern Client *client;
extern int client_size;

int cli_args(int, char**);
void handle_request(char *, int, int);
int client_add(int, uid_t);
void client_del(int);
void loop_select(void);
void handled_request(char *, int, int, uid_t);
void loop_poll(void);
