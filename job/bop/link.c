/*
 * 编程之美与微软100题所有的链表题目汇总
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct link {
    struct link *next;
    unsigned long data;
};


void init(struct link * list,unsigned long value)
{
    if(!list) return;
    
    list->next = NULL;
    list->data = value;
}

/*
 * 在head后面插入链表list
 */
void insert(struct link *head,struct link *list)
{
    struct link *next = head->next;
    
    head->next = list;
    list->next = next;
}

/*
 * 删除链表list
 */
    
struct link* delete_self(struct link *list)
{
    struct link *next = list->next;

    if(next) {
        list->data = next->data;
        list->next = next->next;
    } else
        assert(0); //不能是最后一个结点
    
    init(next,0);
    
    return next;
}
/*
 * 删除prev后面的一个节点
 */
struct link * delete(struct link *prev)
{
    struct link *list = prev->next;
    prev->next = list->next;
    init(list,0);
    return list;
}

void print(const struct link *list)
{
    while(list) {
        printf("next:%p,data:%lu\n",list,list->data);
        list = list->next;
    }
    
}

/*
 * 链表逆序，一次遍历
 */
struct link *  reverse(struct link *list)
{
    struct link *p1,*p2,*p3;
    
    p1 = list;
    p2 = p1->next;
    if(!p2)
        return p1;

    p1->next = NULL;

    p3 = p2->next;
    p2->next = p1;

    printf("p1:%p,data:%lu,p2:%p,data:%lu,p3:%p,data:%lu\n",p1,p1->data,p2,p2->data,p3,p3->data);

    if(!p3)
        return p2;

    while(p3 && p3->next && p3->next->next) {
        p1 = p3;
        p3 = p3->next;
        p1->next = p2;
        
        p2 = p3;
        p3 = p3->next;
        p2->next = p1;
        
        printf("p1:%p,data:%lu,p2:%p,data:%lu,p3:%p,data:%lu\n",p1,p1->data,p2,p2->data,p3,p3->data);
        getchar();
    }

    if(p3) {
        p1 = p3;
        p3 = p3->next;
        p1->next = p2;
        if(p3) {
            p3->next = p1;
            p1 = p3;
        }
        
    } else
        p1->next = p2;
    
    return p1;
}

void test_reverse(void)
{
    struct link list[6],*head;
    int i;
    
    for(i = 0;i < sizeof(list)/sizeof(list[0]);i++)
        init(&list[i],i);
    
    head = list;
    for(i = 1;i < sizeof(list)/sizeof(list[0]);i++)
        insert(head,list+i);
    
    print(head);
    
    head = reverse(list);
    printf("after reverse:\n");
    print(head);
}


int main(int argc,char *argv[])
{
    test_reverse();
    return 0;
}
