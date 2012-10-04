#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

static int set_signal_handler(int sig,void (*handler)(int))
{
    struct sigaction sa;
    struct sigaction old_sa;
    
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_handler = handler;
    sigemptyset(&(sa.sa_mask));
    sa.sa_flags = 0;
    
#ifdef SA_INTERRUPT
    sa.sa_flags |= SA_INTERRUPT;
#endif
    /*
#ifdef SA_RESTART
    sa.sa_flags |= SA_RESTART;
#endif
    */

    if(sigaction(sig,NULL,&old_sa) == -1) {
        fprintf(stderr,"cannot get old signal handler");
        return -1;
    }
    
    if(old_sa.sa_handler == SIG_DFL && sigaction(sig,&sa,NULL) == -1)
    {
        fprintf(stderr,"cannot set signal handler");
        return -1;
    }
    
    return 0;
}


void sigcatcher(int signo)
{
    //printf("PID %d caught one \n",getpid());
    set_signal_handler(SIGINT,sigcatcher);
}

int main(void)
{
    int ppid;
    
    set_signal_handler(SIGINT,sigcatcher);
    
    if((ppid = fork()) == 0) {
        int fd;
        char buf[100];
        
        fd = 0;
        //fd = open("./7_12.c",O_RDONLY);
        for(;;) {
            //lseek(fd,0,SEEK_SET);
            printf("read :%d,str:%s\n",read(fd,buf,100),strerror(errno));
            //getchar();
        }
        
    }
    
    for(;;) 
        if(kill(ppid,SIGINT) == -1)
            exit(0);
    
    return 0;
}
