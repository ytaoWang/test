/**
 * @file   61.c
 * @author wyt <wyt@wyt-laptop>
 * @date   Mon Sep 10 19:01:26 2012
 * 
 * @brief  
 * @email:datao0907@163.com
 * 
 * 找出数组中两个只出现一次的数字
 * 题目：一个整形数组里除了两个数字之外，其它的数字都出现了两次。
 * 请写程序找出这两个只出现一次的数字，要求事件复杂度为O(n),空间复杂度为O(1).
 * 分析：这是一道很新颖的关于位运算的面试题
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 找出数组中一个只出现一次的数字
 */
int OnlyOneNumber(int a[],int size)
{
    int i,num;
    
    num = a[0];
    for(i = 1;i < size;++i) {
        num ^= a[i];
    }
    
    return num;
}

/*
 * 找出数组中两个只出现一次的数字!!
 */
int OnlyTwoNumber(int a[],int size,int *x,int *y)
{
    int i,num;
    
    num = a[0];
    
    for(i = 1;i < size;++i) {
        
    }
    
}


int main(int argc,char *argv[])
{
    int a[] = {1,2,3,4,4,2,3};
    int x,y,b[] = {1,2,3,4,4,2};
    
    printf("OnlyOneNumber:%d\n",OnlyOneNumber(a,sizeof(a)/sizeof(a[0])));
    
    OnlyTwoNumber(b,sizeof(b)/sizeof(b[0]),&x,&y);
    printf("OnlyTwoNumber:%d,%d\n",*x,*y);
    
    return 0;
}

