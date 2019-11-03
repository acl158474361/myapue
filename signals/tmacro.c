#define sigemptyset(ptr) (*(ptr)=0)
#define sigfullset(ptr) (*(ptr)=~(sigset_t)0,0)

