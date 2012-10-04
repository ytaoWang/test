#include <stdio.h>
#include <string.h>

#define SIZE (8*1024*1024)

int main(int argc,char *argv[])
{
    int i;
    char str[SIZE+1];
    
    memset(str,'\0',SIZE+1);
    
    for(i = 0;i < SIZE;++i)
        str[i] = 'a';
    
    return 0;
}
