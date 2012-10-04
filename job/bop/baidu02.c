/*
 * 1.给定如下的n*n数字矩阵,每行从左到右是严格递增,每列的数据也严格
 *  递增
 * 1 2 3
 * 3 5 6
 * 4 8 9
 * 现在要求设计一个算法,给定一个数k,判断k是否在这个矩阵中.描述算法并且给出时间
 * 复杂度
 */

#include <stdio.h>
#include <stdlib.h>

#define N 3

int get_number(int a[][N],int n,int idx)
{
    int row,col;
    
    row = idx/N;
    col = idx%N;

    //printf("idx:%d,col:%d,row:%d,num:%d\n",idx,col,row,a[row][col]);
    
    return a[row][col];
}

int binary_search(int a[][N],int key,int size)
{
    int left ,right,mid;
    
    left = 0;
    right = size * size - 1;
    
    while(left < right - 1) 
    {
        mid = left + (right - left)/2;
        
        if(get_number(a,size,mid)  <= key)
            left = mid;
        else
            right = mid - 1;
    }
    
    if(get_number(a,size,left) == key)
        return left;
    else
        if(get_number(a,size,right) == key)
            return right;
    
    return -1;
}

int c_search(int a[][N],int key,int size)
{
    
    return -1;
}


int main(int argc,char *argv[])
{
    int a[][N] = 
        {
            {1,2,3},
            {3,4,6},
            {7,8,9},
        };
    
    int key = 4;
    
    printf("search key:%d,res:%d\n",key,binary_search(a,key,N));
    
    
    return 0;
}
