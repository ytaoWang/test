/*
 * 颠倒栈
 * 题目:用递归颠倒一个栈，例如输入栈为{1,2,3,4,5},1在栈顶
 * 颠倒之后的栈为:{5,4,3,2,1},5处在栈顶
 */

#include <stdio.h>
#include <stdlib.h>

#define N 100

struct stack {
    int a[N];
    int curr;
};

void init(struct stack *s)
{
    s->curr = 0;
}

int empty(const struct stack *s)
{
    return !s->curr;
}

int full(const struct stack *s)
{
    return s->curr == N;
}


int push(struct stack *s,int element)
{
    if(full(s))
        return -1;
    s->a[s->curr++] = element;

    return 0;
}

int pop(struct stack *s,int *element)
{
    if(empty(s))
        return -1;
    *element = s->a[--s->curr];
    return 0;
}


int flip(struct stack *s)
{
    int element;
    
    if(empty(s))
        return -1;

    pop(s,&element);
    printf("flip pop:%d\n",element);
    
    flip(s);
    printf("flip push:%d\n",element);
    push(s,element);

    return 0;
}

int flip2(struct stack *s1,struct stack *s2)
{
    int element;
    
    if(empty(s1))
        return 0;
    
    pop(s1,&element);
    push(s2,element);
    flip2(s1,s2);
}


int main(int argc,char *argv[])
{
    struct stack s,s1;
    int i,a[] = {1,2,3,4,5};
    
    init(&s);
    init(&s1);
    
    for(i = sizeof(a)/sizeof(a[0]) - 1;i >=0;i--)
        push(&s,a[i]);
    /*
    while(!empty(&s)) {
        pop(&s,&i);
        printf("before filp:%d\n",i);
    }
    */
    /*
    flip(&s);
    
    while(!empty(&s)) {
        pop(&s,&i);
        printf("%d\n",i);
    }
    */
    flip2(&s,&s1);
    while(!empty(&s1)) {
        pop(&s1,&i);
        printf("%d\n",i);
    }

    return 0;
}


