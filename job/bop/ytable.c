/**
 * @file   ytable.c
 * @author wyt <wyt@wyt-laptop>
 * @date   Sat Oct  6 19:19:15 2012
 * 
 * @brief  杨式矩阵Young Table (行列均递增)
 * @email:datao0907@163.com
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 4

#define INF INT_MAX

static void print_table(int y[][N],int row,int col);

static void __swap(int *key1,int *key2)
{
    int tmp = *key1;
    *key1 = *key2;
    *key2 = tmp;
}


static int build_ytable(int y[][N],int m,int n,int row,int col)
{
    //printf("adjust key:y[%d][%d]:%d\n",m,n,y[m][n]);
    if(m== 0 && n ==0)
        return 0;
    
    if(0 == n) {
        if(y[m][n] < y[m-1][n]) 
        {
            __swap(&y[m][n],&y[m-1][n]);
            build_ytable(y,m-1,n,row,col);
        }
        
        return 0;
    }
    
    if(0 == m) {
        if(y[m][n] < y[m][n-1]) {
            __swap(&y[m][n],&y[m][n-1]);
            return build_ytable(y,m,n-1,row,col);
        }

        return 0;
    }

    if(y[m][n] < y[m-1][n]) {
        __swap(&y[m][n],&y[m-1][n]);
        build_ytable(y,m-1,n,row,col);
    }
    
    if(y[m][n] < y[m][n-1]) {
        __swap(&y[m][n], &y[m][n-1]);
        return build_ytable(y,m,n-1,row,col);
    }
    
    return 0;
}

int insert_ytable(int y[][N],int row,int col,int key)
{
    if(y[row-1][col-1] < INF) {
        fprintf(stderr,"ytable is full y[%d][%d]:%d\n",row-1,col-1,y[row-1][col-1]);
        print_table(y,N,N);
        return -1;
    }
    

    y[row-1][col-1] = key;
    return build_ytable(y,row-1,col-1,row,col);
}

int extract_min(int y[][N],int m,int n,int row,int col)
{
    if(m == row-1 && n == col - 1) {
        y[m][n] = INF;
        return 0;
    }
    
    if(m == row - 1) {
        y[m][n] = y[m][n+1];
        return extract_min(y,m,n+1,row,col);
    }
    
    if(n == col - 1) {
        y[m][n] = y[m+1][n];
        return extract_min(y,m+1,n,row,col);
    }
    
    if(y[m+1][n] < y[m][n+1]) {
        y[m][n] = y[m+1][n];
        return extract_min(y,m+1,n,row,col);
    }
    
    y[m][n] = y[m][n+1];
    return extract_min(y,m,n+1,row,col);
}

static int ytable_search(int y[][N],int row,int col,int key,int *r,int *c)
{
    int high,low,mid,i;
    
    *r = -1;
    *c = -1;
    
    //开始从副对角线开始进行二分查找
    for(i = row - 1; i >= 0; i--) {
        low = i;
        high = row - 1;

#ifdef DEBUG
        printf("i:%d,low:%d,high:%d\n",i,low,high);
#endif

        while(low < high) {
            mid = (high + low) >> 1;
#ifdef DEBUG
            printf("low:%d,high:%d,mid:%d\n",low,high,mid);
#endif
            if(y[mid][mid-i] == key) {
                *r = mid;
                *c = mid - i;
                return 0;
            }
                
            if(y[mid][mid-i] < key)
                low = mid + 1;
            else 
                high = mid - 1;
        }

        if(high >= i && y[high][high-i] == key) {
            *r = high;
            *c = high - i;
            return 0;
        }
        
    }
    
    for(i = 1; i < col;i++) {
        low = i;
        high = col - 1;
        while(low < high) {
            mid = (high + low)>>1;
            if(y[mid-i][mid] == key) {
                *r = mid-i;
                *c = mid;
                return 0;
            }
            
            if(y[mid-i][mid] < key)
                low = mid + 1;
            else 
                high = mid - 1;
        }

        if(high >= i && y[high-i][high] == key) {
            *r = high - i;
            *c = high;
            return 0;
        }
        
    }

    return -1;
}


static void print_table(int y[][N],int row,int col)
{
    int i,j;

    for(i = row -1 ;i >= 0;i--) {
        //printf("row:%d\n",i);
        for(j = 0;j < col;j++)
            //printf("y[%d][%d]:%d\t",i,j,y[i][j]);
            if(y[i][j] != INF)
                printf("%d\t",y[i][j]);
            else
                printf("INF\t");
        printf("\n");
    }
}


int main(int argc,char *argv[])
{
    int y[N][N],i,j,r,c;
    int a[] = {9,16,3,2,4,8,5,14,12,3,2,-1};
    
    
    for(i = 0;i < N;i++)
        for(j = 0;j < N;j++)
            y[i][j] = INF;
    
    for(i = 0;i < sizeof(a)/sizeof(a[0]);i++) {

        //printf("insert key:%d\n",a[i]);
        if(insert_ytable(y,N,N,a[i]) < 0)
            fprintf(stderr,"fail to insert key:%d\n",a[i]);
        //print_table(y,N,N);
        //printf("------------------\n");
    }

    print_table(y,N,N);
    
    for(i = 0;i < sizeof(a)/sizeof(a[0]);++i) {
    //for(i = 0;i < 3;++i) {
        if(0 == ytable_search(y,N,N,a[i],&r,&c))
            printf("ytable search %d:row:%d,col:%d\n",a[i],
                   r,c);
        else
            printf("fail to find key:%d,row:%d,col:%d\n",a[i],r,c);
    }
    
    /*
    for(i = 0;i < sizeof(a)/sizeof(a[0]);i++) {
        printf("extract min:%d\n",y[0][0]);
        extract_min(y,0,0,N,N);
    }
    */
    print_table(y,N,N);
    
    return 0;
}
