/*
 * lcs:Largest common substring
 * a: abcdefgh
 * b: aabbcdehh
 * output:bcde
 * d[i][j] = max{d[i-1][j-1] + 1(i>1,j>1,a[i] == b[j]),1(i<=1,j<=1,a[i] == b[j]),0}
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void __kmp_prefix(const char *str,int next[],int);

void lcs1(const char *str1,const char *str2)
{
    int i,j,b,s,len;
    const char *p,*q;
    
    len = 0;
    b = -1;
    s = -1;
    p = str1;
    q = str2;
    
    int d[strlen(p)][strlen(q)];
    for(i = 0,p = str1;*p != '\0';i++,p++)
        for(j = 0,q = str2; *q != '\0';j++,q++)
        {
            if(*p == *q) {
                if(i >0 && j > 0)
                    d[i][j] = 1 + d[i-1][j-1];
                else
                    d[i][j] = 1;
            } else
                d[i][j] = 0;
            if(len < d[i][j]) {
                b = i;
                s = j;
                len = d[i][j];
            }
            
        }
    
    p = str1;
    printf("max substring length:%d,start:%d,string:%s\n",len,b,p+b-len+1);
}

static const char * __kmp_match(const char *pattern,const char *txt,int *size)
{
    int s1,s2,len,i,j;
    const char *p = NULL;
    
    s1 = strlen(pattern);
    s2 = strlen(txt);
    len = 0;
    
    if(size)
        *size = 0;
    
    int next[s1];    
    
    __kmp_prefix(pattern,next,s1);
    j = -1;
    
    /*
    puts(pattern);
    for(i = 0;i < s1;++i)
        printf("%d ",next[i]);
    printf("\n");
    */
    for(i = 0;i < s2;++i) {
        
        while(j >= 0 && pattern[j+1] != txt[i]) {
            j = next[j];
        }
        
        if(pattern[j+1] == txt[i])
            j++;

        if(len < (j+1)) {
            len = j + 1;
            p = txt + i - j;
        }

        if(len == s1) {
            j = next[j];
        }
        
    }

    
    if(size)
        *size = len;
    
    return p;
}


static void __kmp_prefix(const char *str,int next[],int size)
{
    int i,k,s;
    
    for(i = 0;i < size;++i)
        next[i] = -1;
    
    k = -1;
    
    for(s = 1; s < size;++s) {
        /*
         * 为自己建立状态函数next[s] = max{k:Pk 为Ps的最大后缀 }
         */
        while( k >= 0 && str[k+1] != str[s]) {
            k = next[k];
        }
        
        if(str[k+1] == str[s])
            k++;

        next[s] = k;
    }
}

const char *kmp_match(const char *pattern,const char *txt,int *size)
{
    return __kmp_match(pattern,txt,size);
}


/*
 * 采用kmp方法来进行找出最大公共字符串序列
 */
void lcs2(const char *str1,const char *str2)
{
    int s1,s2,i,len;
    const char *p,*q,*s,*p1;

    p = str1;
    q = str2;
    s1 = strlen(str1);
    s2 = strlen(str2);
    //p,s1:pattern,q,s2:txt
    if(s1 > s2) {
        i = s2;
        s2 = s1;
        s1 = i;
        p = str2;
        q = str1;
    }

    s = NULL;
    p1 = NULL;
    len = 0;
    
    while(*p != '\0') {
        p1 = kmp_match(p,q,&i);
        if(len < i) {
            len = i;
            s = p1;
        }
        
        p++;
    }
    
    printf("len:%d,largest common str:%s\n",len,s);
}

static void __swap(const char *str[],int i,int j)
{
    const char *p;

    p = str[j];
    str[j] = str[i];
    str[i] = p;
}


/*
 * 对字符串数组进行排序
 */
static void quick_sort(const char *str[],int left,int right,int (*cmp)(const char *,const char *))
{
    int j,i = 0;
    const char *p = NULL;
    
    if(left >= right)
        return;
    
    __swap(str,left,(left+right)>>1);

    i = left;
    for(j = left+1;j <= right;j++) {
        if((*cmp)(str[left],str[j]) > 0) {
            __swap(str,++i,j);
        }
    }

    __swap(str,left,i);
    
    quick_sort(str,left,i-1,cmp);
    quick_sort(str,i+1,right,cmp);
}

/*
 * second is key
 */
static int __strcmp1(const char *str1,const char *str2)
{
    
    return *str1 - *str2;
}


static int __bsearch(const char *key,const char *str[],unsigned int left,unsigned int right,int (*cmp)(const char *,const char *))
{
    int i,mid;
    
    while(left < right) {

        mid = (left + right)/2;

        i = (*cmp)(str[mid],key);
        if(i <= 0) left = mid + 1;
        else right = mid - 1;
        //puts("hello");
    }
    
    mid = (left + right)/2;
    
    if(!((*cmp)(str[mid],key)))
        return mid;

    return -1;
}

/*
 * longest prefix string
 */
static int __lps(const char *str1,const char *str2)
{
    int len = 0;
    
    while((*str1++ == *str2 ++) != '\0')
        len++;

    return len;
}

/*
 * 所有连续字符串问题均可通过后缀数组来进行实现
 * 假设匹配字符串为:str1:asdfsdaf,str2:ljksfsdasd
 * str1的后缀数组为:
 * asdfsdaf
 * sdfsdaf
 * dfasdaf
 * fasdaf
 * asdaf
 * sdaf
 * daf
 * af
 * f
 * 继续对两个后缀数组进行排序,然后通过对该数组进行查找,则可以找出所有连续的字符串
 */
void lcs3(const char *str1,const char *str2)
{
    int s1,s2,i,j,len,tlen,k;
    
    s1 = strlen(str1);
    s2 = strlen(str2);
    
    const char *p1[s1];
    const char *p2[s2];
    
    for(i = 0;i < s1;i++)
        p1[i] = str1+i;
    
    for(i = 0;i < s2;i++)
        p2[i] = str2+i;

    //quick_sort(p1,0,s1-1,strcmp);
    //quick_sort(p2,0,s2-1,strcmp);
    len = 0;
    k = 0;
    /*
    printf("str1:%s,str2:%s\n",str1,str2);
    
    for(i = 0;i < s1;++i)
        puts(p2[i]);
    
    puts("------p1 suffix array:---------");
    */
    for(i = 0;i < s1;i++) {
        for(j = 0;j < s2;j++) {
            if(len < (tlen = __lps(p1[i],p2[j]))) {
                len = tlen;
                k = i;
            } 
        }
    }
    
    printf("max substring length:%d,%s\n",len,p1[k]);
}

/*
 * 改进版后缀数组,通过将两个后缀放到一个数组中进行排序,直接比较相邻的最大前缀长度
 */
void lcs4(const char *str1,const char *str2)
{
    int s1,s2,i,j,len,tlen;

    s1 = strlen(str1);
    s2 = strlen(str2);
    const char *p[s1 + s2];
    
    for(i = 0;i < s1;++i)
        p[i] = str1 + i;
    
    for(i = 0;i < s2;++i)
        p[s1+i] = str2 + i;
    
    quick_sort(p,0,s1+s2-1,strcmp);
    
    len = 0;
    tlen = 0;
    for(i = 0;i < s1 + s2;++i) {
        if(len < (tlen = __lps(p[i],p[i+1]))) {
            len = tlen;
            j = i;
        }
    }
    
    printf("largest substring length:%d,%s\n",len,p[j]);
}


int main(int argc,char *argv[])
{
    const char *p,*q;
    int i,j,len,b,s;
    
    if(argc != 3) {
        fprintf(stderr,"usage:%s string string\n",argv[0]);
        return -1;
    }
    
    if(strlen(argv[1]) < strlen(argv[2])) {
        p = argv[1];
        q = argv[2];
    } else {
        p = argv[2];
        q = argv[1];
    }
    
    
    lcs1(p,q);
    lcs2(p,q);
    lcs3(p,q);
    lcs4(p,q);
    //p = kmp_match(p,q,&len);
    //printf("%s,len:%d\n",p,len);
    return 0;
}
