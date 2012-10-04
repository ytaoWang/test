/*
 * 32题
 * 有两个序列a,b,大小都为n,序列元素的值任意整数，无序:
 * 要求：通过交换a,b中的元素，使数组a的元素和与数组b元素和 之间的差最小
 * for example:
 * var a = {100,99,98,1,2,3},var b = {1,2,3,4,5,40}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void swap(void *v,int i,int j,int nmemb)
{
    void *tmp;

    if(i == j) return;
    tmp  = (void *)calloc(nmemb,sizeof(char));

    memcpy(tmp,v + i*nmemb,nmemb);
    memcpy(v+i*nmemb,v + j*nmemb,nmemb);
    memcpy(v +j*nmemb,tmp,nmemb);

    free(tmp);
}

static void print(void *v,unsigned int nmemb,unsigned int left,unsigned int right)
{
    int i;
    for(i = left;i<=right;++i) {
        printf("i:%d,element:%d\n",i,*(int *)(v + i * nmemb));
    }
}

static void __merge(void *v,unsigned int l,void *v1,unsigned l1,unsigned int r1,void *v2,unsigned l2,unsigned int r2, \
                    unsigned int nmemb,int (*cmp)(const void *,const void *))
{
    if(l1 > r1 && l2 <= r2) {
        memcpy(v + l * nmemb, v2 + l2 * nmemb,(r2 - l2 + 1) * nmemb);
    } else if(l2 > r2 && l1 <= r1)
        memcpy(v + l * nmemb, v1 + l1 * nmemb,(r1 - l1 + 1) * nmemb);
    else {
        int ret = (*cmp)(v1 + l1 * nmemb,v2 + l2 * nmemb);
        if(ret == 0) {
            memcpy(v + l * nmemb,v1 + l1 * nmemb,nmemb);
            l1++,l++;
            memcpy(v + l * nmemb,v2 + l2 * nmemb,nmemb);
            l2++,l++;
        } else if(ret < 0) {
            memcpy(v + l * nmemb,v1 + l1 * nmemb,nmemb);
            l1++,l++;
        } else {
            memcpy(v + l * nmemb,v2 + l2 * nmemb,nmemb);
            l2++,l++;
        }
        
        __merge(v,l,v1,l1,r1,v2,l2,r2,nmemb,cmp);
    }
    
}


static void __qsort(void *v,unsigned int nmemb,unsigned int left,unsigned int right,int (*cmp)(const void *,const void *))
{
    unsigned int i,last;
    
    if(right <= left) return;

    swap(v,right,(left+right)>>1,nmemb);
    last = left-1;
    
    for(i = left;i< right;++i)
        if((*cmp)(v+right * nmemb,v+i*nmemb) > 0) {
            ++last;
            swap(v,last,i,nmemb);
        }

    last++;
    swap(v,last,right,nmemb);
    
    if(last > left)
        __qsort(v,nmemb,left,last-1,cmp);
    if(last < right)
        __qsort(v,nmemb,last+1,right,cmp);

}

/** 
 * sort array v by quicksort
 * 
 * @param v array
 * @param nmemb array minal unit
 * @param size array's size
 * @param cmp compare one element with other
 */
void sort(void *v,unsigned int size,unsigned int nmemb,int (*cmp)(const void *,const void *))
{
    __qsort(v,nmemb,0,size-1,cmp);
}

void* merge(void *v1,unsigned int s1,void *v2,unsigned int s2,unsigned int nmemb,int (*cmp)(const void *,const void *))
{
    int i,j,k,ret;
    void *v = calloc(s1+s2,nmemb);
    i = 0;
    j = 0;
    k = 0;
    
    while(j < s1 && k < s2)
    {
        ret = (*cmp)(v1 + j * nmemb,v2 + k * nmemb);
        
        if(ret < 0) {
            memcpy(v+i * nmemb,v1+j*nmemb,nmemb);
            ++i,++j;
        } else if(ret == 0) {
            memcpy(v + i * nmemb,v1+j * nmemb,nmemb);
            i++,j++;
            memcpy(v + i * nmemb,v2+k * nmemb,nmemb);
            i++,k++;
        } else {
            memcpy(v + i * nmemb,v2 + k * nmemb,nmemb);
            ++i;
            ++k;
        }
    }
    
    if(j < s1)
        memcpy(v + i * nmemb,v1 + j * nmemb,(s1 - j) * nmemb);
    else if(k < s2)
        memcpy(v + i * nmemb,v2 + k * nmemb,(s2 - k) * nmemb);
    
    return v;
}

void merge2(void *v,void *v1,unsigned int s1,void *v2,unsigned int s2,unsigned int nmemb,int (*cmp)(const void *,const void *))
{
    __merge(v,0,v1,0,s1 - 1,v2,0,s2 -1,nmemb,cmp);
}

static void __difference(int v[],unsigned int size,int v1[],unsigned int idx,unsigned int v2[],unsigned int visited[],int sum,int *diff)
{
    int s1,i;
    static int num = 0;
    
    if(idx == size/2) {
        s1 = 0;
        num++;
        for(i = 0;i < size/2;i++)
            s1 += v1[i];
        if(abs((sum - s1) - s1) < *diff) {
            for(i = 0;i < idx;++i)
                v2[i] = v1[i];
            *diff = sum - 2 * s1;
        }
        puts("------new--------");
        print(v2,sizeof(int),0,idx - 1);
        printf("diff:%d,s1:%d,sum:%d,sum - s1:%d,num:%d\n",*diff,s1,sum,sum - s1,num);
        //getchar();
        return;
    }
    
    for(i = idx ;i < size;++i) {
        if(!visited[i]) {
            v1[idx++] = v[i];
            visited[i] = 1;
            break;
        }
    }

    if(i != size) {
        __difference(v,size,v1,idx,v2,visited,sum,diff);

        idx--;
        visited[i] = 1;
        __difference(v,size,v1,idx,v2,visited,sum,diff);
        visited[i] = 0;
        
        return;
    }

    //for(i = idx;i < size;i++)
    //    visited[i] = 0;
    idx--;

}

static void __filter_difference(int v[],unsigned int size,int v1[],unsigned int idx,unsigned int v2[],unsigned int visited[],int sum,int *diff,int s)
{
    int i;
    static int num = 0;
    
    if(idx == size/2) {
        num ++;
        
        if((sum %2 == 1 && *diff == 1) || (sum % 2 == 0 && *diff == 0))
            return;
        
        if((sum/2 - s) <= *diff) {
            for(i = 0;i < idx;i++)
                v2[i] = v1[i];
            *diff = sum -  2*s;
        }
        //print(v2,sizeof(int),0,idx - 1);
        //printf("diff:%d,s:%d,sum:%d,sum - s1:%d,num:%d\n",2 * *diff,s,sum,sum - s,num);
        //getchar();
        return;
    }
    
    for(i = idx;i < size;++i) {
        if(!visited[i] && (v[i]+s) <= sum/2) {
                v1[idx++] = v[i];
                s += v[i];
                visited[i] = 1;
                break;
        }
    }
    
    /*
     *
     * add some restriction for search
     */
    if( i != size) {
        ;
    }
    
    
    if(!((sum %2 == 1 && *diff == 1) || (sum % 2 == 0 && *diff == 0)) && i != size) {
        __filter_difference(v,size,v1,idx,v2,visited,sum,diff,s);
        idx--;
        visited[i] = 1;
        s -= v[i];
        __filter_difference(v,size,v1,idx,v2,visited,sum,diff,s);
        visited[i] = 0;

        return;
    }
    
    idx--;
}


void difference3(int a[],unsigned int size)
{
    int sum,i,j,v,k,n;
    
    sum = 0;
    for(i = 0;i < size;i++)
        sum += a[i];
    /*
     * b[k][v]:whether b is decomposed equal to number of k in v or not
     * b[k][v] = b[k-1][v-a[k]] v>=a[k]
     */
    unsigned int b[size/2+1][sum/2];
    
    for(i = 0;i <= size/2;i++)
        for(j = 0;j <= sum/2;++j)
            b[i][j] = 0;

        b[0][0] = 1;

        n = size/2;
        
        for(k = 0;k < size;++k)
        {
            for(i = k < n?k:n;i>0;i--)
                for(v=1;v <= sum/2;v++)
                {
                    if(v >= a[k] && b[i-1][v - a[k]])
                        b[i][v] = 1;
                }
        }

        n = sum;

        for(j = sum/2;j>=0;j--)
            if(b[size/2][j])
                break;
        
        printf("sum:%d,one:%d,anther:%d,diff:%d\n",sum,j,sum - j,sum - 2 * j);
}


void max_difference(int v[],unsigned int size)
{
    int i;
    int sum = 0,diff;
    int v1[size/2],visited[size],v2[size/2];
    
    for(i = 0; i < size;++i) {
        sum += v[i];
    }

    for(i = 0;i < size/2;++i) {
        v1[i] = 0;
        v2[i] = 0;
        visited[i] = 0;
        visited[i+size/2] = 0;
    }
    
    diff = sum;
    
    __difference(v,size,v1,0,v2,visited,sum,&diff);

    printf("%d\n",diff);
    print(v2,0,size/2-1,sizeof(int));
}

void min_filter_difference(int v[],unsigned int size)
{
    int i,sum,s,diff;
    int v1[size/2],visited[size],v2[size/2];
    
    sum = 0;
    for(i = 0;i < size;++i)
        sum += v[i];
    s = 0;
    
    for(i = 0;i < size/2;++i) {
        v1[i] = 0;
        v2[i] = 0;
        visited[i] = 0;
        visited[i+size/2] = 0;
    }
    
    diff = sum;
    
    __filter_difference(v,size,v1,0,v2,visited,sum,&diff,s);

    printf("diff:%d,sum:%d\nv2:\n",diff,sum);
    print(v2,sizeof(int),0,size/2-1);
}


void min_difference(int v[],unsigned int size)
{
    int i,j,sum,d,diff;
    int median,sub;
    
    sum = 0;
    for(i = 0;i < size;++i)
        sum += v[i];
    
    median = sum >> 1;
    sub = 0;
    d = size>>1;
    diff = median - sub;
    
    for(i = 0,j = size-1;d != 0 && diff != 0;)
    {
        if(d == 1) {
            int tmp,idx;
            tmp = diff;
            while(i <=j && diff >0) {
                if(v[j] > diff) j--;
                else if(v[i] == diff) {   
                    sub += v[i];
                    diff -= v[i];
                    break;
                } else if(v[j] == diff) {
                    sub += v[j];
                    diff -= v[j];
                    break;
                }
                
                if((diff - v[i]) < tmp) {
                    tmp = v[i];
                    idx = i;
                } else i++;
                
                if((diff - v[j]) < tmp) {
                    tmp = v[j];
                    idx = j;
                } else j--;
            }

            if(!diff) break;
            sub += tmp;
            diff -= tmp;
            d--;
        } else if(d == 2) {
            while(v[j] > diff) j--;
            sub += v[j];
            diff -= v[j--];
            printf("v[%d]:%d\n",j,v[j]);
        } else if((v[j] + v[i]) < diff) {
            diff -= v[i];
            sub += v[i++];
            diff -= v[j];
            sub += v[j--];
            d -=2;
            printf("choose v[%d]:%d,v[%d]:%d\n",i+1,v[i+1],j+1,v[j+1]);
        } else {
            printf("d:%d,v[%d]:%d,v[%d]:%d,vi+vj:%d,diff:%d\n",d,i,v[i],j,v[j],v[i]+v[j],diff);
            j--;
        }
        
    }
    
    printf("min difference:%d,sum:%d,sub:%d\n",sum -  2 *sub,sum,sub);
}


static int cmp(const void *s1,const void *s2)
{
    int n1,n2;
    
    n1 = *(int *)s1;
    n2 = *(int *)s2;
    
    return n1 - n2;
}

int main(int argc,char *argv[])
{
    //int v1[] = {5,3,14,1,20,-15};
    //int v2[] = {1,2,32,4,54,12};
    int v1[] = {5,3,14,1,20,-15,11,18,27,32,52,19,36,35,43};
    int v2[] = {1,2,32,4,54,12,19,24,66,80,67,72,49,31,27};
    //int v1[] = {1,5,7,8,9};
    //int v2[] = {6,3,11,20,17};
    
    int s1,s2;
    
    s1 = sizeof(v1)/sizeof(v1[0]);
    s2 = sizeof(v2)/sizeof(v2[0]);
    
    int v[s1 + s2];
    
    int left,right,i,nmemb;
    
    sort(v1,sizeof(v1)/sizeof(v1[0]),sizeof(int),cmp);
    sort(v2,sizeof(v2)/sizeof(v2[0]),sizeof(int),cmp);
    
    left = 0;
    right = sizeof(v1)/sizeof(v1[0])-1;
    nmemb = sizeof(int);

    puts("v1:");
    print(v1,nmemb,left,right);
    
    puts("v2:");
    print(v2,nmemb,left,right);
    
    merge2(v,v1,right + 1,v2,right+1,sizeof(int),cmp);
    puts("");
    print(v,nmemb,left,sizeof(v)/sizeof(v[0])-1);
    
    void *tmp;
    tmp = merge(v1,right+1,v2,right+1,sizeof(int),cmp);
    puts("");
    print(tmp,nmemb,left,s1+s2 - 1);

    min_filter_difference(v,s1+s2);
    difference3(v,s1+s2);
    free(tmp);
    return 0;
}

