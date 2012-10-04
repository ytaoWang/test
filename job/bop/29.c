/*
 * 栈的push,pop序列
 * 题目:输入两个整数序列。其中一个序列表示栈的push顺序，判断另一个序列有没有可能是对应的pop顺序
 * 为了简单起见，我们假设push序列的任意两个整数都是不相等的。
 */


#include <stdio.h>
#include <stdlib.h>

#define N 10

struct stack {
    int a[N];
    int curr;
};

static void init(struct stack *s)
{
    s->curr = 0;
}

static int empty(struct stack *s)
{
    return !s->curr;
}

static int full(struct stack *s)
{
    return s->curr == N;
}


static int pop(struct stack *s,int *element)
{
    if(empty(s))
        return -1;
    
    if(element)
        *element = s->a[--s->curr];
    else 
        --s->curr;
    
    return 0;
}

static int push(struct stack *s,int element)
{
    if(full(s))
        return -1;

    s->a[s->curr++] = element;
    return 0;
}

static int top(struct stack *s)
{
    return s->a[s->curr-1];
}

int is_sequence(int a[],const int b[],int size)
{
    int k,i = 0;
    struct stack s;
    
    k = 0;
    
    init(&s);
    push(&s,a[i++]);
    
    while(!empty(&s)) {
        if(i >= size && top(&s) != b[k])
            return 0;
        if(top(&s) == b[k]) {
            pop(&s,NULL);
            k++;
        } else 
            push(&s,a[i++]);
    }
    
    return 1;
}


int main(int argc,char *argv[])
{
    int a[] = {1,2,3,4,5};
    int b[] = {5,4,3,2,1};

    
    printf("sequence:%d\n",is_sequence(a,b,sizeof(a)/sizeof(a[0])));
    printf("sequence:%d\n",is_sequence(b,a,sizeof(a)/sizeof(a[0])));

    return 0;
}
