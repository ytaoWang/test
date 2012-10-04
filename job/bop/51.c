#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_sequence(int num,unsigned int idx,unsigned mid)
{
    int left,right,i;
    if( idx % 2)  //奇数
        //printf("idx:%d,mid:%d\n",idx,mid);
        left = mid - ((idx - 1)>>1);
    else
        left = mid - idx/2 + 1;

        if(left <= 1) 
            return 1;

        if((left + left + idx - 1) * idx/2 != num)
            return 0;
        
        printf("idx:%d\t",idx);
        for(i = 0;i < idx;i++)
            printf("%d ",left + i);
    

    printf("\n");
    return left <= 1;
}


int find_sequence(int num)
{
    int i;
    for(i = 2;i < (num+1)/2;i++) {
        if(print_sequence(num,i,num/i))
            break;
    }
    
    return 1;
}
    

int main(int argc,char *argv[])
{
    int num;
    
    if(argc != 2) {
        fprintf(stderr,"usage:%s num\n",argv[0]);
        return -1;
    }
    num = atoi(argv[1]);
    num > 1 &&find_sequence(num);
    
    return 0;
}
