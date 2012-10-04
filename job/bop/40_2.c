/*
 * 珠子的首尾相连问题
 * 
 *一串首尾相连的珠子(m个)，有N中颜色(N <= 10)
 * 设计一个算法，取出一段，要求包含所有N中颜色，并使长度最短。
 * 并分析时间复杂度与空间复杂度
 */

#include <stdio.h>
#include <stdlib.h>


/*
 * 获取颜色索引的起始点
 */
int min_index(int b[],int color)
{
    int idx,i;
    
    i = 0;
    idx = b[i++];
    
    for(;i < color;++i)
        if(b[i] < idx)
            idx = b[i];

    return idx;
}


/*
 * 目前未想到合适的动态规划算法，先尝试用状态机思想来完成，当作字符串扫描算法来进行处理
 * 这种多状态的问题不能采用动态规划来进行处理，因为这里的每一个状态需要依靠不止一个状态，而是过去的n个状态。
 * 如果N=2，则可以采用动态规划处理，否则就不能采用，多个状态的变迁就需要记录每个状态的变化，也就超越了动态规划的使用范畴
 */

int pearl_length(int a[],int size,int color)
{
    int b[color];
    int f,i,num,len,j,t;
    
    f = size;
    num = 0;
    len = size;
    
    for(i = 0;i < color;i++) {
        b[i] = -1;
    }
    
    /*
     * 假设颜色的珠子不是环
     */
    for(i = 0;i < size;++i) {
        
        if(num == color) {   
            f = min_index(b,color);
            if((i - f) < len)
                len = i - f;
            //printf("from:%d,to:%d\n",f,i);
        }
        /*
         * 如果是第一次更新索引，更新长度
         */
        if(b[a[i]] < 0) {
            b[a[i]] = i;
            num++;
        } else {
            /*
             * 第二次就是更新长度了
             */
            b[a[i]] = i;
        }
    }
    
    /*
     * 假设颜色的珠子解就是一个环
     */
    for(i = 0;i < color;i++)
        b[i] = -1;
    f = size;
    num = 0;
    j = size-1;
    i = 0;
    b[a[i]] = size + i;
    num ++;
    
    if(b[a[j]] < 0) {
        num++;
        b[a[j]] = j;
    }
    
    b[a[j]] = j;
    i++,j--;
    
    while(i < j && (b[a[i]] < 0 || b[a[j]] <0)) {
        if(b[a[i]] < 0) {
            b[a[i]] = size + i;
            num++;
            i++;
        }
        
        if(b[a[j]] < 0) {
            b[a[j]] = j;
            num++;
            j--;
        }
    }
    
    if(num == color) {
        i--;
        f = min_index(b,color);
        printf("from:%d,i:%d\n",f,size + i);
        if((size + i - f + 1) < len)
            len = size + i - f + 1;
        return len;
    }
    
    /*
     * 找出从i开始的完整颜色的珠子长度
     */
    f = num;
    t = i;
    while(i <=j  && num != color) {
        if(b[a[i]] < 0) {
            b[a[i]] = size + i;
            num++;
        } else if(b[a[i]] >= size)
            b[a[i]] = size + i;
        i++;
    }
    
    if((size + i - min_index(b,color)) < len)
        len = size + i - min_index(b,color);
    
    num = f;
    
    /*
     * 找出从j结尾的完整最大珠子长度
     */
    while(j >= t && num != color) {
        if(b[a[j]] < 0) {
            b[a[j]] = j;
            num ++;
        } else if(b[a[j]] < size)
            b[a[j]] = j;
        
        j--;
    }
    
    if((size + t - min_index(b,color)) < len)
        len = size + t - min_index(b,color);
    
    return len;
}

/*
 * 采用状态机方式来实现
 */
int peral_length2(int a[],int size,int color)
{
    int i,from,to,b[color],num,f;
    
    from = 0;
    to = 0;
    num = 0;
    
    for(i = 0;i < size;i++)
        b[i] = -1;
    b[a[from]] = from;
    to = from + 1;
    num++;
    f = from;
    
    while(from ! = size) {
        
        if(num == color) {
            f = min_index(b,color);
            if((to - f) < len)
                len = i - f;
            from = f;
        }
        
        if(b[a[to]] < 0)
            num++;

        b[a[to]] = to++;

    }
    
    return len;
}



int main(int argc,char *argv[])
{
    int a[] = {2,3,1,1,1,0,1,1,0,1,2};
    
    printf("pearl length:%d\n",pearl_length(a,sizeof(a)/sizeof(a[0]),4));
    printf("pearl length:%d\n",pearl_length2(a,sizeof(a)/sizeof(a[0]),4));

    return 0;
    
}
