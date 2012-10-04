/*
 * 1.百度笔试:给定一个存放整数的数组，重新排列数组使得数组左边为奇数，右边为偶数.
 *   要求：空间复杂度O(1),时间复杂度为O(n)
 * 2.用C语言实现函数void * memmove(void *dest,const void *src,size_t n).
 *  memmove函数的功能是拷贝src所指的内存内容前n个字节到dest所指的地址上。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int odd_even(int a[],int size)
{
    int i,j,tmp;
    
    i = 0;
    j = size-1;
    
    while(i < j)
    {
        while(a[i] % 2) i++;
        while(!a[j] % 2) j--;
        
        tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
    
    return 0;
}

void * memmove(void *dest,const void *src,size_t n)
{
    char tmp[n];
    
    memcpy((void *)tmp,src,n);
    memcpy(dest,(void *)tmp,n);
    
    return dest;
}


int main(int argc,char *argv[])
{
    int i;
    int a[] = {1,1,2,5,5,3};
    
    odd_even(a,sizeof(a)/sizeof(a[0]));
    
    for(i = 0;i < sizeof(a)/sizeof(a[0]);i++)
        printf("%d ",a[i]);
    printf("\n");
    return 0;
}


