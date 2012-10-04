/*
 * 一个数组整数,利用空间复杂度O(1),时间复杂度O(1)来进行移动整数,要求负数在前面,正数在后面,相对位置不变
 * 如:-3,1,2,-1,-3,4 ---> -3,-1,-3,1,2,4
 **/

#include <stdio.h>
#include <stdlib.h>

void insert_move(int a[],int size)
{
    int minus,plus,i,k,tmp;
    int pb = 0;
    
    minus = -1;
    plus = -1;
    
    for(i = 0;i < size;i++)
    {
        if(a[i] < 0) {
            minus = i;
        } else if(a[i] > 0 && !pb) {
            plus = i;
            pb = 1;
        }
        
        if(plus >= 0 && plus < minus) {
            
            tmp = a[minus];
            
            for(k = minus;k >= plus;k--)
                a[k] = a[k-1];
            minus = plus; //reset next time
            a[plus++] = tmp;
        }
    }
    
}


int main(int argc,char *argv[])
{
    int a[] = {3,-1,2,-1,-3,4};
    int i;
    
    insert_move(a,sizeof(a)/sizeof(a[0]));
    
    for(i = 0;i < sizeof(a)/sizeof(a[0]);i++)
        printf("%d ",a[i]);
    printf("\n");
    
    
    return 0;
}
