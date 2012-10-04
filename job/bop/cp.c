/*
 * 写出strncpy,memcpy函数实现
 */
#include <stdio.h>

void* memset(void *dst,int n,size_t size)
{
    char *p = (char *)dst;
    
    while( p < ((char *)dst + size))
        *p++ = n;

    return dst;
}


static void * memcpy(void *dst,const void *src,size_t size)
{
    const char *p = (const char *)src;
    char *q = (char *)dst;
    
    if(!q) return NULL;
    
    if(p == q) return dst;
    /*
     * p:    ----------
     * q:------- 
     */
    if(q <= p + size) {
        while(p != ((const char *)src + size))
            *q ++ = *p ++;

        return dst;
    }

    /*
     * p: -----------
     * q:    -----------
     */
    p = p + size - 1;
    q = q + size - 1;
    
    while((const char *)src <= p)
        *q -- = *p --;

    return dst;
}

char *strncpy(char *dst,const char *src,size_t size)
{
    const char *p = src;
    char *q = dst;

    /*
     * p:   ----------
     * q:-------
     */
    if(q <= p + size) {
        while(p != src + size) 
            *q++ = *p++;

        return dst;
    }
    
    p += size - 1;
    q += size - 1;
    
    while(src <= p) 
        *q -- = *p --;
    
    return dst;
}



int main(int argc,char *argv[])
{
    char src[30];
    char dst[] = "hello,world";
    
    memset(src,0,30);

    memcpy(src,dst,sizeof(dst));
    puts(src);
    
    strncpy(src,src+3,sizeof(src));

    puts(dst);
    
    return 0;
}

