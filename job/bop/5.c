/*
 * 查找最小的k个元素
 * 题目:输入n个整数，输出最小的k个元素
 * 例如输入1,2,3,4,5,6,7和8这8个数字，则最小的4个数字为:1,2,3,4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE(a)  (sizeof(a)/sizeof(a[0]))

static void swap(int a[],int i,int j)
{
    int tmp;

    tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

static void print(int a[],int from,int to)
{
    while(from <= to) {
        printf("a[%d]:%d\n",from,a[from]);
        from++;
    }
    
}


/*
 * 这类问题最简单的当然是排序问题，但又完全不是排序
 * 这类问题可以转化为求第k个最小数问题，该问题又可以通过快排思想的分治法实现
 */
int min_k(int a[],int from,int to,int *b,int num)
{
    int k,i;
    
    if(to < from)
        return -1;
    else if(from == to) {
        if(from == num)
            *b = a[from];
        return from;
    }
    
    //找出第num个小数(递增顺序排序)
    swap(a,from,(from + to)/2);
    k = from;
    for(i = from + 1;i <= to; ++i) {
        if(a[i] < a[from]) {
            k++;
            swap(a,i,k);
        }
    }
    
    swap(a,k,from);
    
    //printf("k:%d,num:%d,a[%d]:%d,from:%d,to:%d\n",k,num,k,a[k],from,to);
    //print(a,from,to);
    //刚好找到第num下的数
    if(k == num) {
        *b = a[k];
        return k;
    } else if(k < num) //这就需要从右端开始找了(类似二分查找中的右端寻找)
        return min_k(a,k+1,to,b,num);
    else 
        return min_k(a,from,k-1,b,num); //还是从左边开始找吧
}

static void __swap(void *v1,void *v2,int nmemb)
{
    void *tmp = calloc(1,nmemb);
    
    memcpy(tmp,v1,nmemb);
    memcpy(v1,v2,nmemb);
    memcpy(v2,tmp,nmemb);
    
    free(tmp);
}


/*
 * 通过旋转来判断堆的合法性
 */
static void rotate_heap(void *v,int parent,int size,int nmemb,int (*cmp)(void *,void *))
{
    int left,right,largest;
    
    left  = 2 * parent + 1;
    right = 2 * parent + 2;
    
    largest = parent;
    
    if(left < size && (*cmp)(v + largest * nmemb,v + left * nmemb) < 0) largest = left;
    if(right < size && (*cmp)(v + largest * nmemb,v + right * nmemb) < 0) largest = right;
    
    if(largest != parent) {
        __swap(v + largest * nmemb,v + parent * nmemb,nmemb);
        printf("swap:%d,%d\n",*(int *)(v + largest * nmemb),*(int *)(v + parent * nmemb));
        rotate_heap(v,parent/2,size,nmemb,cmp);
    }
}

static int int_cmp(void *s1,void *s2)
{
    int ret;
    
    ret =  *(int *)s1 - *(int *)s2;
    
    //printf("cmp:s1:%d,s2:%d\n",*(int *)s1,*(int *)s2);
    return ret;
}


/*
 * 通过建立一个大顶堆，共k个元素,当新元素小于顶层元素时，加入到堆中,否则丢弃,
 * 但当删除堆元素直到该堆剩下k个为止，返回堆顶元素
 */
int min_k_heap(int a[],int from,int to,int *b,int num,int h[])
{
    int size = to - from +1;
    int i;    

    for(i = 0;i < size;i++) h[i] = 0;
    printf("size:%d\n",size);

    for(i = from;i <= to;i++) {
        if((i - from + 1) <= num) {
            h[i - from] = a[i];
            printf("i:%d,%d\n",i,a[i]);
            rotate_heap(h,(i - from - 1)/2,i-from+1,sizeof(int),int_cmp);
        } else if(h[0] > a[i]) {
            /*
             * 如果有更小的元素，则将最大的元素替换最小的元素
             */
            h[0] = a[i];
            rotate_heap(h,0,num,sizeof(int),int_cmp);
        }
    }
    
    print(h,from,i);
    
    return 0;
}


/*
 * 通过计数排序原理来实现
 */
int count(int a[],int from,int to,int *b,int num,int h[])
{
    
}



int main(int argc,char *argv[])
{
    int a[] = {3,4,2,1,23,4,5};
    int d[] = {3,4,2,1,23,4,5};
    int i,b[SIZE(a)],c,num,idx;
    
    c = 5;
    num = 0;
    
    for(i = 0;i < SIZE(b);i++)
        b[i] = 0;
    for(i = c-1,idx = SIZE(a) - 1;i>=0;i--) {
        idx = min_k(a,0,idx,&num,i);
        b[i] = num;
    }
    
    printf("min %d,num:%dth:%d\n",c,idx+1,a[idx]);
    for(i = 0;i < c;++i)
        printf("%d\n",b[i]);
    
    printf("min %d,heap:%d\n",c,min_k_heap(d,0,SIZE(d) - 1,&num,c,b));
    for(i = 0;i < c;i++)
        printf("%d\n",b[i]);

    return 0;
}
