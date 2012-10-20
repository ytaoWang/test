/*
 * 背包问题总结,主要针对的是0/1背包问题,其中主要分为两类:
 * 1.物品只能取一次(可以不用装满/必须装满)
 * 2.物品可以重复取多次(可以不用装满/必须装满)
 *
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 物品只能取一次,而且可以不用装满
 * 动规思想 f[i][d]:装前i个物品时的重量为d的最大价值
 * f[i][d] = max{f[i-1][d - w[i]]+v[i],f[i-1][d]}
 * f[d] = max{f[d - w[i]]+v[i],f[d]}
 * 输入按重量递增排序(可忽略)
 */
void knapsack11(const int w[],const float v[],int size,int weight)
{
    int i,d;
    float f[size][weight+1];
    int prev[size][weight+1];
    
    for(i = 0;i < size;i++)
        for(d = 0;d <= weight;d++)
            prev[i][d] = 0;
    
    for(i = 0;i < size;i++) {
        for(d = 0;d < size;d++) {
            f[i][d] = 0.0;
            prev[0][d] = 0;
            f[0][d] = v[0];
        }
    }
    
    float max = 0.0;
    
    for(d = 1;d <= weight;++d) 
        for(i = 1;i < size;++i) {
            f[i][d] = f[i-1][d];
            if(d >= w[i] && f[i][d] < (f[i-1][d - w[i]] + v[i])) {
                f[i][d] = f[i-1][d - w[i]] + v[i];
                //prev[i][d] = 1;
            }
            if(max < f[i][d]) {
                prev[i][d] = 1;
                max = f[i][d];
            }
        }

    printf("max value:%f,max:%f\n",f[size-1][weight],max);

    d = weight;
    while(d > 0) {
        for(i = size - 1;i >= 0;--i)
            if(f[i][d] != f[i-1][d]) {
                printf("v[%d]:%f,weight:%d\n",i,v[i],w[i]);
                d -= w[i];
            }
    }
    
}

void  knapsack111(const int w[],const float v[],int size,int weight)
{
    int i,d;
    float f[weight+1];
    int prev[size][weight+1];
    
    for(i = 0;i < size;i++)
        for(d = 0;d <= weight;d++)
            prev[i][d] = 0;
    
    for(i = 0;i < size;i++) {
        for(d = 0;d < size;d++) {
            f[d] = 0.0;
            prev[0][d] = 0;
            f[d] = v[0];
        }
    }
    
    float max = 0.0;
    
    for(d = 1;d <= weight;++d) 
        for(i = 1;i < size;++i) {
            f[d] = f[d-1];
            if(d >= w[i] && f[d] < (f[d - w[i]] + v[i])) {
                f[d] = f[d - w[i]] + v[i];
            }

            if(max < f[d]) {
                prev[i][d] = 1;
                max = f[d];
            }
        }

    printf("max value:%f,max:%f\n",f[weight],max);
    /*
    d = weight;
    while(d > 0) {
        for(i = size - 1;i >= 0;--i)
            if(f[i][d] != f[i-1][d]) {
                printf("v[%d]:%f,weight:%d\n",i,v[i],w[i]);
                d -= w[i];
            }
    }
    */
}

int main(int argc,char *argv[])
{
    int w[] = {1,2,3,4,5,6,7};
    float v[] = {0.15,0.2,0.2,0.5,0.4,0.4,0.55};
    
    knapsack111(w,v,sizeof(w)/sizeof(w[0]),10);
    
    return 0;
}
