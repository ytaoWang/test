/*
 * 在从1到n的正数中1出现的个数
 * 题目:输入一个整数n,求从1到n这n个整数的十进制表示中1出现的次数。
 * 例如：输入12,从1到12这些整数中包含1的数字有1,10,11,和12,1一共出现了5次。
 * 分析：这是一道广为流传的google面试题
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * 在分析数字问题时,在感到束手无策时,首先就是通过示例进行计算,找出数字的规律,再下手写程序
 */

#include <stdio.h>
#include <stdlib.h>

unsigned int digit(int value)
{
    unsigned int n = 1;
    
    while(value != 0) {
        value /= 10;
        n++;
    }
    
    return n;
}

unsigned long power(int radix,unsigned int p)
{
    unsigned long value = 0;
    
    if(p == 0) return 1;
    else if(p == 1) return radix;
    
    //奇数
    if(p % 2) {
        value = power(radix,(p-1)>>1);
        return radix * value * value;
    }

    value = power(radix,p>>1);
    return value * value;
    
}


unsigned int total(unsigned int d)
{
    int i,n = d;
    
    i = 1;
    
    while(i <= d) {
        n *= 10;
        i--;
    }
    
    return n;
}

/*
 * d位为1时,1的个数
 */
int number(int a[],int size,int d)
{
    int i,n = 1;
    unsigned int value = 0;

    if(d == size - 1) {
        if(a[d] > 1) {
            for(i = 0; i < size - 1;i++)
                n *= 10;
            return n;
        }

        for(i = size - 2;i >= 0;i--) {
            value = value * 10 + a[i];
        }
        
        return value + 1;
    }
    
    
    if(a[d] > 1) {
        //最高位选除了小于目前数的所有位
        n = (a[size - 1] - 1) * power(10,size-2);
        //最高位为目前数字
        for(i = d+1,value = 1;i < size-1;i++) {
            if(a[i] > 0)
                value *= a[i];
        }
        
        value += power(10,d);
        
        return n + value;
    }
    
    
}


int count(int value)
{
    unsigned int n,d = digit(value);
    int i,a[d],v;
    
    if(d == 1 && value > 0)
        return 1;
    /*
     * 求出0 ~ (d-1)位数的所有1之和
     */
    n = total(d-1);
    i = 0;
    
    /*
     * 求出最后d位数的1之和
     */
    v = value;
    while(value != 0) {
        a[i++] = value % 10;
        value /= 10;
    }
    
    for(i = 0;i < d;i++) {
        n += number(a,d,i);
    }
    
    return n;
}

int count2(unsigned int value)
{
    unsigned int n,factor,radix,d;
    
    n = 1;
    
    if(value > 0 && value < 10) 
        return n;
    
    value /= 10;
    radix = 10;
    d = 1;
    
    while(value) {
        
        factor = value % 10;
        
        if(factor != 1)
            n += radix;

        if(!factor) 
            factor = 1;
        
        value /= 10;

        n *= factor;
        
        n += d;
        
        d++;
        radix *= 10;
    }
    
    return n;
}


unsigned int count3(unsigned int value)
{
    unsigned int count,factor,current,low,high;
    
    factor = 1;
    count = 0;
    
    while( value / factor)
    {
        current = (value  / factor) % 10;
        low = value % factor;
        high = value / (10 * factor);

        printf("current:%u,low:%u,high:%u,factor:%u\n",current,low,high,factor);
        
        switch(current) {
        case 1:count += high * factor + low + 1;break;
        case 0:count += high  * factor; break;
        default:count += (high + 1) * factor; break;
        }
        
        factor *= 10;
    }
    
    return count;
}



int main(int argc,char *argv[])
{
    printf("Number 2243 of 1:%d\n",count2(2243));
    printf("Number 100 of 1:%d\n",count3(100));
    printf("Number 100 of 1:%d\n",count3(13));
    
    return 0;
}
