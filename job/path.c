/*
 * 已知一个矩阵n*n 求出从左上角到右下角的所有路径
 * 如: 1*1 2,2*2 12,3*3 184
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//n阶
#define N (6+1)
static uint64_t count = 0;

int traverse(int x,int y,int v[],int idx,int visited[][N])
{
    int i;
    
    if(x == N-1 && y == N-1) {
#ifdef DEBUG
        printf("-----------------\n");
        printf("(0,0)\n");
        for(i = 0;i < idx;i++) {
            printf("(%d,%d)\n",v[i]/N,v[i]%N);
        }
        printf("(%d,%d)\n",N-1,N-1);
        printf("now:%d\n",count);
#endif
        ++count;
        return 0;
    }
    
    //查找顺序:右下左上
    //find from right
    if(y%N != N-1 && !visited[x][y+1]) {
        v[idx++] = x*N + y + 1;
        visited[x][y+1] = 1;
        traverse(x,y+1,v,idx,visited);
        idx--;
        visited[x][y+1] = 0;
    }
    //find key from below
    if(x != N-1 && !visited[x+1][y]) {
        v[idx++] = (x+1)*N + y;
        visited[x+1][y] = 1;
        traverse(x+1,y,v,idx,visited);
        idx--;
        visited[x+1][y] = 0;
    }
    
    //find from left
    if(y%N != 0 && !visited[x][y-1]) {
        v[idx++] = x*N + y-1;
        visited[x][y-1] = 1;
        traverse(x,y-1,v,idx,visited);
        idx--;
        visited[x][y-1] = 0;
    }
    //find from above
    if(x != 0 && !visited[x-1][y]) {
        v[idx++] = (x-1) * N + y;
        visited[x-1][y] = 1;
        traverse(x-1,y,v,idx,visited);
        idx--;
        visited[x-1][y] = 0;
    }
    
    visited[x][y] = 0;
    idx--;
    
    return 0;
}


int main(int argc,char *argv[])
{
    int i,v[N * N],j;
    int visited[N][N];
    
    for(i = 0;i < N;++i)
        for(j = 0;j < N;++j) {
            visited[i][j] = 0;
            v[i * N + j] = 0;
        }

    visited[0][0] = 1;
    //visited[N-1][N-1] = 1;
    traverse(0,0,v,0,visited);
    printf("%d total:%llu\n",N-1,count);
    
    return 0;
}
