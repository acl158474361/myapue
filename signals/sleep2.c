#include<setjmp.h>
#include<signal.h>
#include<unistd.h>
#include<stdio.h>
static jmp_buf env_alrm;
static void sig_alarm(int signo){
    longjmp(env_alrm,1);
}

unsigned int sleep2(unsigned int seconds){
    signal(SIGALRM,sig_alarm);
    if(setjmp(env_alrm) == 0){
        alarm(seconds);
        pause();
        printf("pause finished\n"); //只有从别的信号处理程序返回才会调用这段代码
       
    }
    return alarm(0);
}