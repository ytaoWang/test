/*
 * 编程求解:
 * 输入两个整数n和m，从数列1,2,3,....n中任意取几个数，使其和等于m，要求将其中所有可能的组合列出来
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * 组合数学中的选与不选例子
 */
int subsum(int n,int a[],int cur,int m,int idx)
{
    int i;
    
#ifdef DEBUG
    printf("m:%d,cur:%d,idx:%d,n:%d\n",m,cur,idx,n);
#endif

    if(m < 0 || idx > n) // || (m != 0 && cur > m))
        return 0;

    else if(0 == m) {
        for(i = 0;i < idx;i++)
            printf("%d ",a[i]);
        printf("\n");
        
        return 0;
    }

    for(i = cur;i < n;i++)
        if(i <= m)
            break;
    
    if(i != n) {
        a[idx] = i;
        subsum(n,a,i+1,m-i,idx+1);
    }
    
    if(cur <= m)
        subsum(n,a,cur+1,m,idx);
    
    return 0;
}



int main(int argc,char *argv[])
{
    int a[10],n,m;
    
    n = 10;
    m = 11;
    
    printf("n:%d,m:%d\n",n,m);
    
    subsum(n,a,1,m,0);
    
    return 0;
}

