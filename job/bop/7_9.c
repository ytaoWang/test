#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void catcher(int sig)
{
}



int main(int argc,char *argv[])
{
    signal(SIGINT,catcher);
    kill(0,SIGINT);
    
    return 0;
}
