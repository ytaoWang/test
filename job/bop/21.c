/*
 * 编程求解:
 * 输入两个整数n和m，从数列1,2,3,....n中任意取几个数，使其和等于m，要求将其中所有可能的组合列出来
 */
#include <stdio.h>
#include <stdlib.h>


int subsum(int n,int a[],int size,int m,int idx)
{
    int i;
    
    if(m < 0)
        return 0;
    else if(!m) {
        for(i = 0;i < idx;i++)
            printf("%d ",a[i]);
        return 0;
    }
    
    for(i = idx;i < n;i++) {
        if(i < m)
            break;
    }
    
    if(i != n) {
        a[idx++] = i;
        
    }
    
}
