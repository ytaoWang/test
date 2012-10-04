/*
 * 求数组的子数组之和的最大值
 * 一个有N个整数元素的一维数组(A[0],A[1],...,A[n-2],A[n-1]),这个数组当然有很多子数组，
 * 那么子数组之和的最大值是什么呢?
 *
 */

/*
 * 最值问题多半采用动态规划来完成，但在分析最优化子问题需要分析子问题的前缀特点，一般来讲前缀存在的形式有如下：
 * 1.如果是连续问题，则前缀形成的递归式为i 与 i+1 项之间的关系
 * 2.非连续问题，子问题就成了i 与 j之间的问题
 * 3.如果是非连续，又有限定条件(背包问题)，就需要采用i项"选“与”不选“之间的关系，写成递归式
 * 4.上面涉及的都是单数组之间的前缀分析，如果涉及到两个以上的数组，则定义的子问题又可上面的三种形式：
 * 5.子问题的前缀均为连续形式，则递归式就是每个数组之间i项与i+1项之间的关系
 * 6.子问题的前缀非连续形式，则递归式中就是每个数组之间的i项与j项之间的关系
 * 7.子问题的前缀在有限制的情况下，则递归式就是每个数组之间的i项”选“与”不选“之间的关系
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 这里由于是连续问题，则之间处理i项与i+1项之间的关系
 * s[i]:以i项开始的连续和问题
 * s[i+1] = max{s[i] + a[i+1]}
 * 但是s[i+1] < 0 的话，就可以忽略掉，这样就可以不用s[N]了
 */
int max_subarray(int a[],int size)
{
    int sum,i,max;
    
    sum = a[0];
    max = a[0];
    
    for(i = 1;i < size;++i) {
        if(sum <0)
            sum = 0;
        sum += a[i];
        if(sum > max)
            max = sum;
    }
    
    return max;
}

/*
 * 现在是数组为一个环形结构，最大连续和又该如何实现?
 * 1.最简单的方法：数组循环移位，再进行逐一求解最大连续和,算法复杂度O(n^2)
 * 2.s[i][l] = s[i][l-1]+a[i+l-1]; sum = max{s[i][l]},s[i][l]:以i开始长度为l的连续和
 * O(n) 的解法:
 * 1.解未出现跨域的情况，也就是a[0] ~ a[n-1](原问题)
 * 2.解出现跨域情况 a[n-1]~a[0] 出现在解的中间或边界
 */

int max_subarray_bound(int a[],int from,int to)
{
    int i,max,sum;
    
    max = a[from];
    sum = a[from];
    
    for(i = from + 1;i <= to;i++) {
        if(sum < 0)
            sum = 0;
        sum += a[i];
        if(max < sum)
            max = sum;
    }

    return max;
}


int max_subarray_cycle(int a[],int size)
{
    int sum,i,j,s,m;
    
    //第一种情况,未跨域
    sum = max_subarray_bound(a,0,size-1);
    /*
     * 第二种情况，出现跨域,既然a[0] a[n-1]都出现在解中，则可以将其解延伸到最大，
     * 也就是从a[0]开始，向右扫描，从a[n-1]开始向左扫描,两者直到出现负数为止，
     * 这样就不会出现中间是负数，但是两端是正数，但却选择整个序列的情况.
     */

    if(size == 2) return sum;

    i = 1;
    j = size-2;

    s = a[i-1] + a[j+1];
    
    while(i < j && (a[i] >=0 || a[j] >= 0)) {
        if(a[i] >= 0) {
            s += a[i];
            i++;
        }
        
        if(a[j] >= 0) {
            s += a[j];
            j--; 
        }
    }
    
    if(i == j) {
        if(a[i] >0) s+=a[i];
        if(s < sum) return sum;
        else return s;
    }
    
    if(i < j) {
        //算出以i为头部的所有连续子数组和或者以j为尾的连续子数组和,从而找到最大的元素
        int s2,k;
        m = a[i];
        s2 = m;
        for(k=i+1;k <= j;k++) {
            s2 += a[k];
            if(m < s2)
                m = s2;
        }
        
        s2 = a[j];
        for(k = j-1;k>=i;k--) {
            s2 += a[j];
            if(m < s2)
                m = s2;
        }
        
        if(m > 0)
            s += m;
    }
    
    if(s < sum) return sum;
    else return s;
    
}


int main(int argc,char *argv[])
{
    //int a[] = {1,-3,20,-4,-10,11};
    int a[] = {5,-3,-1,5,10,11};

    printf("max sub array:%d\n",max_subarray(a,sizeof(a)/sizeof(a[0])));
    printf("max sub array(cycle):%d\n",max_subarray_cycle(a,sizeof(a)/sizeof(a[0])));

    return 0;
}
