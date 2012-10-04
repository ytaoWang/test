/*
 * 设计包含一个min 函数 的栈,定义栈的数据结构，要求添加一个min的函数，能够得到栈的最小元素
 * 要求函数min,push及pop的时间复杂度均为O(1)
 */

#include <stdio.h>
#include <stdlib.h>

#define N 10

struct stack {
    int a[N];
    int cura,curb;
    int b[N];
};

int init(struct stack *s)
{
    s->cura = 0;
    s->curb = 0;
}


int push(struct stack *s,int element)
{
    if(s->cura == N)
        return -1;
    s->a[s->cura++] = element;
    if(!s->curb || s->b[s->curb - 1] > element)
        s->b[s->curb++] = element;

    return 0;
}

int pop(struct stack *s,int *element)
{
    if(s->cura == 0)
        return -1;
    *element = s->a[--s->cura];
    if(*element == s->b[s->curb - 1])
        s->curb--;

    return 0;
}

int min(struct stack *s,int *element)
{
    if(!s->curb)
        return -1;
    
    *element =  s->b[s->curb - 1];

    return 0;
}


int main(int argc,char *argv[])
{
    struct stack s;
    int a[] = {-10,2,3,4,23,9,-1,2,5};
    int i,m;
    
    init(&s);
    
    for(i = sizeof(a)/sizeof(a[0]) - 1;i >= 0;i--)
    {
        push(&s,a[i]);
        if(!min(&s,&m)) {
            printf("before pop min:%d\n",m);
        } else
            printf("before pop fail to min:%d\n",i);
        
        if((i % 2)) {
            pop(&s,&m);
            if(!min(&s,&m)) {
                printf("min:%d\n",m);
            } else
                printf("fail to min:%d\n",i);
        }
    }
    
    
    return 0;
}
