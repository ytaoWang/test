#include <stdio.h>
#include <stdlib.h>

int p(int k,int a[],int size)
{
    int i,tmp;
    
    if(k == size) {
        for(i = 0;i < size;++i) {
            printf("%d ",a[i]);
        }
        printf("\n");
        
        return 0;
    }
    

    for(i = k;i < size;++i) {
        tmp = a[k];
        a[k] = a[i];
        a[i] = tmp;
        
        p(k+1,a,size);
        
        tmp = a[k];
        a[k] = a[i];
        a[i] = tmp;
        
    }
    
    return 0;
}


int main(int argc,char *argv[])
{
    int a[] = {0,1,2,3,4};
    
    p(0,a,sizeof(a)/sizeof(a[0]));
    
    return 0;
}
