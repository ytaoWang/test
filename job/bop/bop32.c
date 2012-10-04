/*
 * 排列问题,给出一个数n,求出n的所有排列
 * 编程之美32题:电话号码对应英语单词
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static inline void swap(int v[],int i,int j)
{
    int tmp;
    
    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}


static inline void print(int v[],int size)
{
    int i;
    
    for(i = 0;i < size;++i)
        printf("%d ",v[i]);
    printf("\n");
}


/*
 * 首先递归形式给出排列 n! = n * (n - 1)!
 * 通过记录访问历史来进行回溯式求排列
 */

int permution(int k,int v[],int size)
{
    int i,tmp;
    
    if(k == 0) {
        print(v,size);
        return 0;
    }
    
    for(i = k;i >= 0; --i) {
        swap(v,i,k);
        permution(k-1,v,size);
        swap(v,i,k);
    }
    
    return 0;
}


int permution2(int k,int v[],int size)
{
    int i;
    
    //printf("k:%d\n",k);
    
    if(k == size) {
        print(v,size);
        return 0;
    }

    /*
     * 与组合算法相同,都是从当前索引开始
     */
    for(i = k;i < size;i++) {
        swap(v,i,k);
        permution2(k+1,v,size);
        swap(v,i,k);
    }
    
    return 0;
}

/*
 * 这里采用省略下一个访问记录的方式,而是通过循环方式进行遍历(类似电话号码方式进行)
 * 历史记录必不可少!!
 */
int permution4(int k,int h[],int v[],int size)
{
    int i;
    
    if(k == size) {
        print(v,size);
        k--;
        return 0;
    }
    
    for(i = 0;i < size;i++) {
        if(!h[i]) {
            h[i] = 1;
            v[k] = i;
            permution4(k+1,h,v,size);
        }
    }
    
    if(i == size) {
        //printf("k:%d,v:%d\n",k,v[k]);
        h[v[k]] = 0;
        k--;
        if(k>=0)
            h[v[k]] = 0;
    }
    
    return 0;
}


/*
 * 非递归实现阶层函数(尾递归改为非递归形式)
 * 将深度遍历看作是一棵树,通过对该树进行遍历,找出规律:
 * 主要是记录历史记录,当前序列的同时,如何来进行提示下一个遍历的起始点
 * 树的深度遍历有三点:每个位置的历史记录,当前遍历的记录,及当前记录的下一个开始遍历的起始点
 * 如果处理得当,可以通过当前记录来寻找下一个记录,又可以通过当前记录来作为历史记录
 */
int permution3(int a[],int v[],int size)
{
    int i,j,n;
    int h[size],next[size];
    
    for(i = 0;i < size;i++) {
        h[i] = 0;
        v[i] = 0;
        next[i] = 0;
    }
    
    n = 0;
    while(n >= 0) {

        if(n == size) {
            print(v,size);
            n--;
        }

        for(j = next[n];j < size;j++)
            if(!h[j]) {
                v[n++] = j;
                h[j] = 1;
                break;
            }
        
        if(j == size) {
            h[v[n]] = 0;
            v[n] = 0;
            next[n] = v[n];
            n--;
            h[v[n]] = 0;
            next[n] = v[n]+1;
        }
    }
    
    return 0;
}

static int is_equal(const int v[],int key,int size)
{
    int i;

    for(i = 0;i < size;++i)
        if(v[i] == key) return 1;
    return 0;
}


int permution5(int a[],int v[],int size)
{
    int i,j,k;
    
    for(i = 0;i < size;i++)
        v[i] = -1;
    
    k = 0;
    while(k >= 0) {

        if(k == size) {
            print(v,size);
            k--;
        }
        
        for(i = v[k] + 1; i < size;i++) {
            /*
             * 这里通过当前记录来作为历史记录
             * 当前记录通过处理作为下一位置的起始记录
             */
            if(!is_equal(v,i,k)) {
                v[k++] = i;
                break;
            }
            
        }
        
        if(i == size) {
            v[k] = -1;
            k--;
        }
    }
    
}


#define N 10
unsigned char number[][3] = 
        {
            {'\0','\0','\0'},
            {'\0','\0','\0'},
            {'a','b','c'},
            {'d','e','f'},
            {'g','h','i'},
            {'j','k','l'},
            {'m','n','o'},
            {'p','q','r'},
            {'s','t','u'},
            {'v','w','x'},
            {'y','z','0'}
        };

void print_char(int num[],int str[],int size)
{
    int i;
    
    for(i = 0;i < size;++i)
        printf("%c ",number[num[i]][str[i]]);
    printf("\n");
}

/*
 * 递归版本实现树的深度遍历
 * 历史记录,当前记录,下一个遍历起始点(直接从0开始进行遍历,因为每个节点的空间均独立,这个就可以省略了)
 * 通过历史记录可以剔除重复记录
 */
int lookup_word(int k,int h[N][3],int num[],int current[],int size)
{    
    int i;

    if(k == size) {
        print_char(num,current,size);
        k--;
        return 0;
    }
    
    for(i = 0;i < 3;i++) {
        if(!h[num[k]][i] && number[num[k]][i] != '\0') {
            current[k] = i;
            h[num[k]][i] = 1;
            lookup_word(k+1,h,num,current,size);
        }
    }
    
    if(i == 3) {
        for(i = 0;i < 3;++i)
            h[num[k]][i] = 0;
        k--;
        for(i = 0;i < 3;++i)
            h[num[k]][i] = 0;
    }
    
    return 0;
}
/*
 * 这里就弱化历史记录,来不剔除对应的相同记录,因为每个节点的空间均是独立的所以,这里连历史记录都不用保持!!
 * 历史记录的作用是针对各个节点共享一个解空间而言的,如果解空间为非共享,则完全可以不同处理
 * 妙
 */
int lookup_word2(int k,int num[],int current[],int size)
{
    int i;
    
    if(k == size) {
        print_char(num,current,size);
        k--;
        return 0;
    }
    
    for(i = 0;i < 3;i++) {
        if(number[num[k]][i] != '\0') {
            current[k] = i;
            lookup_word2(k + 1,num,current,size);
        }
    }
    
    if(i == 3)
        k--;
}

/*
 * 非递归版本,由于每个节点空间独立,则不用记录历史,但需要记录下一个节点搜索的起始位置
 */
int lookup_word3(int num[],int size)
{
    int v[size],i,k;
    
    k = 0;
    for(i = 0;i < size;i++)
        v[i] = -1;
    
    while(k >= 0) {
        
        if(k == size) {
            print_char(num,v,size);
            k--;
        }
        
        for(i = v[k] + 1;i < 3;++i) {
            if(number[num[k]][i] != '\0') {
                v[k++] = i;
                break;
            }
        }
        
        if(i == 3) {
            v[k] = -1;
            k--;
        }
        
    }
    
    return 0;
}

/*
 * 非递归版本,由于每个节点空间独立,则不用记录历史,但需要记录下一个节点搜索的起始位置,
 * 但是需要相同的节点值不能一致,所以又需要历史记录(也可以通过当前记录来对其进行处理)来降低搜索空间
 */
int lookup_word4(int num[],int size)
{
    int v[size],i,k,j;
    int h[N][size];
    
    k = 0;
    for(i = 0;i < size;i++) {
        for(j = 0; j < N;j++)
            h[j][i] = 0;
        v[i] = -1;
    }
    
    while(k >= 0) {
        
        if(k == size) {
            print_char(num,v,size);
            k--;
        }
        
        for(i = v[k] + 1;i < 3;++i) {
            if(!h[num[k]][i] && number[num[k]][i] != '\0') {
                h[num[k]][i] = 1;
                v[k++] = i;
                break;
            }
        }
        
        if(i == 3) {
            /*
             * 清除记录时,需要全部清除掉
             */
            for(i = 0;i <= v[k];i++)
                h[num[k]][i] = 0;
            v[k] = -1;
            k--;
        }
        
    }
    
    return 0;
}

int main(int argc,char *argv[])
{
    int v[] = {1,2,3,4,5,6};
    //int v[] = {2,6,6,7,8,8,3,7};
    int a[] = {1,2,3,4,5,6};
    int h[N][3],i,j;
    int size = sizeof(v)/sizeof(v[0]);
    int current[size];
    
    for(i = 0;i < size;++i) { 
        current[i] = 0;
        //v[i] = 0;
    }
    
    
    //print(v,sizeof(v)/sizeof(v[0]));
    //permution(size-1,v,size);
    //permution2(0,v,sizeof(v)/sizeof(v[0]));
    //permution3(a,v,size );
    //permution4(0,current,v,size);
    permution5(a,v,size);
    
    for(i = 0;i < N;i++) {
        for(j = 0;j < 3;j++)
            h[i][j] = 0;
        current[i] = 0;
    }
    
    //lookup_word(0,h,v,current,size);
    //lookup_word2(0,v,current,size);
    //lookup_word4(v,size);
    
    return 0;
}
