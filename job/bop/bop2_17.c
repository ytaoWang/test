/*
 * 此题与微软面试100题中第48题类似
 * 一个数组是由一个递减数列左移若干位形成的,如{4,3,2,1,6,5} 是由{6,5,4,3,2,1}左移两位形成的,在这种数组中
 * 查找某一个数
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * 48题
 */
static int __bsearch(int a[],int size,int key)
{
    int low,high,mid;
    
    low = 0;
    high = size - 1;
    
    while(low < high) 
    {
        mid = (low + high) >> 1;
        
        if(a[mid] == key) return mid;
        
        //low <--> high 之间为递增序列,则中间必存在一个转折点low <-- point(递减) --> high(递增)
        if(a[low] < a[high]) {
            if(a[mid] < a[low]) { //low <--> mid 递减序列
                if(a[mid] < key && a[low] >= key)
                    high = mid - 1;
                else 
                    low = mid + 1;
            } else {  // mid <---> high  递减序列
                if(a[mid] < key && a[high] >= key)
                    low = mid - 1;
                else
                    high = mid + 1;
            }
        } else {
            //low <--> high 为递减序列
            if(a[mid] < key)
                high = mid - 1;
            else 
                low = mid + 1;
        }
    }

    if(a[low] == key)
        return low;

    return -1;
}

/*
 * 设计一个算法,把一个含有N个元素的数组循环右移K位,要求复杂度为O(N),且只允许使用两个附加变量
 */
static void reverse(char str[],int b,int e)
{
    char tmp;
    
    while(b < e) {
        tmp = str[b];
        str[b] = str[e];
        str[e] = tmp;
        b++,e--;
    }
}

static void __right_shift(char str[],int size,int k)
{
    k %= size;
    reverse(str,0,size-1-k);
    reverse(str,size-k,size-1);
    reverse(str,0,size-1);
}

/*
 * 微软面试100题第10题
 * 翻转句子中单词的顺序 
 * 题目:输入一个英文句子,翻转句中单词的顺序,但单词内字符的顺序不变
 * I am a student. ---> student. a am I
 */

static void __str_reverse(char str[],int size)
{
    int i,last;
    
    reverse(str,0,size-1);
    
    for(i = 0,last = 0; i < size;++i) {
        if(str[i] == ' ') {
            reverse(str,last,i-1);
            last = i+1;
            continue;
        }
    }
    
}

/*
 * 14题:
 * 题目:输入一个已经按升序排序过的数组和一个数字,在数组中查找两个数,
 * 使得它们的和正好是输入的那个数字.要求时间复杂度为O(n),如果有多对数字的和等于输入的数字,输出任意一对即可.
 */
static void __subsum(int a[],int size,int sum)
{
    int b,e;
    
    b = 0,e = size - 1;
    
    while(b < e) {
        
        if(sum <= a[e]) e--;
        if(sum == (a[b] + a[e])) {
            printf("sum(%d):%d+%d\n",sum,a[b],a[e]);
            return;
        }
        if(a[b] < (sum - a[e]))
            b++;
        else 
            e--;
    }

    printf("fail to find sum:%d\n",sum);
    
}

void set_bits(unsigned long *v,int bits)
{
    *v |= (1UL << bits);
}

void clear_bits(unsigned long *v,int bits)
{
    *v &= ~(1UL << bits);
}

int isset_bits(unsigned long v,int bits)
{
    return v & (1UL << bits);
}

/*
 * 63题 在字符串中删除特定的字符
 * 题目:输入两个字符串,从第一个字符串中删除第二个字符串中所有的字符.如:They are students 和 aeiou
 * 删除之后的第一个字符串就变成"Thy r stdnts"
 */
static void __str_del(char str1[],int s1,const char *str2,int s2)
{
    unsigned long visited[4];
    int i;
    const char *p;
    
    for(i = 0;i < 4;++i)
        visited[i] = 0;
    
    p = str2;
    while(*p != '\0') {
        set_bits(&visited[*p/128],(*p)%128);
        p++;
    }
    
    for(i = 0;i<s1;i++) {
        if(isset_bits(visited[str1[i]/128],str1[i]%128))
            str1[i] = '\0';
    }
    
    for(i = 0;i < s1;i++) 
        if(str1[i] != '\0')
            printf("%c",str1[i]);
}


int main(int argc,char *argv[])
{
    int size;
    int a[] = {7,6,5,4,30,20,10,8};
    char str[] = "I am a student.";
    int b[] = {1,2,4,7,11,15};
    
    int idx,key;
    
    key = 8;
    idx = __bsearch(a,sizeof a/sizeof(a[0]),key);
    if(idx < 0)
        printf("fail to key:%d,res:%d\n",key,idx);
    else
        printf("key:%d,res:%d,idx:%d\n",key,a[idx],idx);

    printf("original str:%s\n",str);
    size = sizeof(str)/sizeof(str[0]) - 1;
    
    // __right_shift(str,sizeof(str)/sizeof(str[0]) - 1,2);
    __str_reverse(str,size);
    printf("now:%s\n",str);
    
    __subsum(b,sizeof(b)/sizeof(b[0]),9);
    
    char str1[] = "They are students";
    int s1 = sizeof(str1)/sizeof(str1[0]) - 1;
    const char *str2 = "aeiou";
    
    __str_del(str1,s1,str2,strlen(str2));
    puts("");
    
    return 0;
}

