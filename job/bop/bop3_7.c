/*
 * 3.7 队列中取最大值问题
 * 假如有这样一个拥有3个操作的队列
 * 1.EnQueue(v):将v加入队列中
 * 2.DeQueue(v):使队列中的队首元素删除并返回此元素
 * 3.MaxElement:返回队列中的最大元素
 * 4,MinElement:返回队列中的最小元素
 */


#include <stdio.h>
#include <stdlib.h>

#define N 10

struct stack {
    int a[N];
    int cura,curb;
    int b[N];
};


struct queue {
    struct stack q1;
    struct stack q2;
};

    

static int __init(struct stack *s)
{
    s->cura = 0;
    s->curb = 0;

    return 0;
}

static int __full(struct stack *s)
{
    return s->cura == N;
}


static int __push(struct stack *s,int element)
{
    if(s->cura == N)
        return -1;
    s->a[s->cura++] = element;
    if(!s->curb || s->b[s->curb - 1] > element)
        s->b[s->curb++] = element;

    return 0;
}

static int __pop(struct stack *s,int *element)
{
    if(s->cura == 0)
        return -1;
    *element = s->a[--s->cura];
    if(*element == s->b[s->curb - 1])
        s->curb--;

    return 0;
}

static int __min(struct stack *s,int *element)
{
    if(!s->curb)
        return -1;
    
    *element =  s->b[s->curb - 1];

    return 0;
}
    
static int __empty(const struct stack *s)
{
    return !s->cura;
}

int init(struct queue *q)
{
    __init(&q->q1);
    __init(&q->q2);
    
    return 0;
}


int enqueue(struct queue *q,int element)
{
    return __push(&q->q1,element);
}

int dequeue(struct queue *q,int *element)
{
    int tmp;
    
    if(!__empty(&q->q2))
        return __pop(&q->q2,element);

    while(!__empty(&q->q1) && !__full(&q->q2)) {
        __pop(&q->q1,&tmp);
        __push(&q->q2,tmp);
    }
    
    return __pop(&q->q2,element);
}

int min(struct queue *q,int *tmp)
{
    int t;
    
    __min(&q->q1,tmp);
    
    if(!__min(&q->q2,&t))
        *tmp = (*tmp < t?*tmp:t);
    
    return 0;
}


int main(int argc,char *argv[])
{
    struct queue s;

    int a[] = {-10,2,3,4,23,9,-1,2,5};
    int i,m;
    
    init(&s);
    
    for(i = sizeof(a)/sizeof(a[0]) - 1;i >= 0;i--)
    {
        enqueue(&s,a[i]);
        if(!min(&s,&m)) {
            printf("before dequeue min:%d,enqueue:%d\n",m,a[i]);
        } else
            printf("before dequeue fail to min:%d\n",i);
        
        if((i % 2)) {
            dequeue(&s,&m);
            if(!min(&s,&m)) {
                printf("min:%d\n",m);
            } else
                printf("fail to min:%d\n",i);
        }
    }
    
    return 0;
}
