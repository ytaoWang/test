#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    printf("hello,");
    execl("/bin/sh","",NULL);
    printf("world\n");
    
    return 0;
}

    
