/*
 * 与93题相同
 * 一个int数组，里面数据无任何限制，要求求出所有这样的数a[i],
 * 其左边的数都小于等于它，右边的数都大于它
 * 能否用一个额外的数组和少量其它空间实现。
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * 用函数的观点分析其最小值
 * 满足条件：left <= a <= right,所以最好需要记录两个数组，
 * 其中记录从左到右记录最大值，而另一端从右到左记录最小值
 * 这里其实用到函数的观点：a大于left，就要大于其最大值，
 * a小于right，就要小于其最小值.
 */
int partition(int a[],int size)
{
    int i,b[2][size],len;

    len = 0;
    for(i = 0;i < size;++i) {
        b[0][i] = INT_MIN;//从左到右记录最大值
        b[1][i] = INT_MAX;//从右到左记录最小值
    }

    b[0][0] = 0;
    for(i = 1;i < size;i++)
        if(a[b[0][i]] < a[i])
            b[0][i] = i;
        else
            b[0][i] = b[0][i-1];
    

    b[1][size-1] = size-1;
    for(i = size-2;i>=0;i--)
        if(a[b[1][i+1]] > a[i])
            b[1][i] = i;
        else
            b[1][i] = b[1][i+1];
    
    
    if(a[0] <= a[b[1][1]]) {
        printf("0,right min:%d\n",a[b[1][1]]);
        len++;
    }
    
    if(a[size-1] >= a[b[0][size-2]]) {
        printf("%d\n",size-1);
        len++;
    }
    
    #ifdef DEBUG
    printf("only first,last,len:%d\n",len);
    #endif

    for(i = 1;i < size-1;++i) {
        if((a[b[0][i-1]] <= a[i]) && (a[i] <= a[b[1][i+1]])) {
            len++;
            printf("%d\n",i);
            #ifdef DEBUG
            printf("idx:%d,left max:%d,right min:%d\n",   \
                   i,b[0][i-1],b[1][i+1]);
            #endif
        }
        
    }
    
    return len;
}

int main(int argc,char *argv[])
{
    int a[] = {10,0,1,3,4,59};
    
    printf("partition num:%d\n",partition(a,sizeof(a)/sizeof(a[0])));
    
    return 0;
}
