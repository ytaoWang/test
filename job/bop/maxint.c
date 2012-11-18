#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define N 20

int cmp(const void *s1,const void *s2)
{
    const char *str1,*str2;
    
    str1 = s1;
    str2 = s2;
    
    return strcmp(str1,str2);
}


void max_int(const int a[],int size)
{
    int i;
    char b[N][N];
    
    assert(size <= N);
    
    for(i = 0;i < size;++i) {
        memset(b[i],'\0',N);
        snprintf(b[i],N,"%d",a[i]);
    }
#ifdef DEBUG    
    
    for(i = 0;i < size;++i)
        printf("before %s\n",b[i]);
#endif  
  
    qsort(b,size,N*sizeof(char),cmp);

    for(i = 0;i < size;++i)
        printf("%s",b[i]);
    
    printf("\n");
}


int main(int argc,char *arv[])
{
    
    int a[]= {1,2,34,12,45};
    
    max_int(a,sizeof(a)/sizeof(a[0]));
    
    return 0;
}
