/*
 * 35:求一个矩阵中最大的二维矩阵(元素和最大).如:
 * 1 2 0 3 4
 * 2 3 4 5 1
 * 1 1 5 3 0
 * 
 * 中最大的是:
 * 4 5
 * 5 3
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define N 3
#define M 5

/*
 * s[i][j]:从第i行，j列一直到第row-1行，col-1列的元素和
 */
int max_submatrix(int a[][M],int row,int col)
{
    int s[row][col];
    int i,j,max;
    
    s[row-1][col-1] = a[row-1][col-1];
    max = s[row-1][col-1];
    
    for(i = row-1;i >= 0;i--)
        for(j = col-1;j >= 0;j--)
        {
            if((i+1) >= row && (j+1) >= col) {
                s[i][j] = a[i][j];
            } else if((i+1)>= row)
                s[i][j] = s[i][j+1] + a[i][j];
            else if((j+1)>= col)
                s[i][j] = s[i+1][j] + a[i][j];
            else
                s[i][j] = s[i+1][j] + s[i][j+1] - s[i+1][j+1] + a[i][j];
            
            if(s[i][j] > max)
                max = s[i][j];
        }
    
    return max;
}

int main(int argc,char *argv[])
{
    int a[N][M] = 
        {
            {1,2,0,3,-4},
            {2,-30,4,5,1},
            {1,1,5,3,0}
        };
    
    printf("max submatrix:%d\n",max_submatrix(a,N,M));
    
    return 0;
}
