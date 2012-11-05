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

unsigned int length(const struct link *list)
{
    unsigned int len = 0;
    
    while(list->next) {
        list = list->next;
        len++;
    }
    
    return len;
}


void print(const struct link *list)
{
    while(list) {
        printf("next:%p,data:%lu\n",list,list->data);
        list = list->next;
    }
    
}

const struct link * intersection(const struct link *list1,const struct link *list2)
{
    unsigned int l1,l2,step;
    const struct link *p,*q;
    
    l1 = length(list1);
    l2 = length(list2);
    p = NULL;
    q = NULL;
    
    /*
     * 链表长度长的先走(l1 - l2)步
     */
    if(l1 < l2) {
        step = l2 - l1;
        p = list2;
        q = list1;
    } else {
        step = l1 - l2;
        p = list1;
        q = list2;
    }
    
    while(step > 0) {
        p = p->next;
        step--;
    }
    /*
     * 共同走步数，找出交点
     */
    while(p && q && p != q) {
        p = p->next;
        q = q->next;
    }
    
    return p;
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

void test_intersection(void)
{
    struct link list1[4],list2[2],list3[3];
    struct link *head1,*head2,*head3;
    
    int i;
    
    for(i = 0;i < sizeof(list1)/sizeof(list1[0]);++i)
        init(&list1[i],i);
    for(i = 0;i < sizeof(list2)/sizeof(list2[0]);++i)
        init(&list2[i],i*2);
    for(i = 0;i < sizeof(list3)/sizeof(list3[0]);++i)
        init(&list3[i],i*3);
    
    head1 = list1;
    head2 = list2;
    head3 = list3;
    
    for(i = 1;i< sizeof(list1)/sizeof(list1[0]);i++)
        insert(head1,list1+i);
    for(i = 1;i< sizeof(list2)/sizeof(list2[0]);i++)
        insert(head2,list2+i);
    for(i = 1;i< sizeof(list3)/sizeof(list3[0]);i++)
        insert(head3,list3+i);
    
    insert(list1+1,head3);
    insert(list2+1,head3);
    
    printf("intersection point:%p,head3:%p\n",\
           intersection(list1,list2),head3);
    
}


int main(int argc,char *argv[])
{
    //test_reverse();
    test_intersection();
    return 0;
}
