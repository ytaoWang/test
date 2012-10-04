#include <stdio.h>
#include <stdlib.h>

#define N 6
int count = 0;

static void print(int a[],int size)
{
	int i;
	printf("current size:%d\n",size);
	for(i = 0;i < size;++i)
		printf("a[%d]:%d ",i,a[i]);
	printf("\n");
}
/*
 * n:剩余的数值
 * sum:现在整数的和
 */
int decompose2(int n,int a[],int next[],int size,int sum,int idx,int current)
{
	int i;
	if(n == 0) {
		//printf("decompose2,sum:%d,n:%d\n",sum,n);
		for(i = 0;i < idx;i++)
			printf("%d ",a[i]);
		printf("\n");
		count++;
		return 0;
	}

	for(i = next[idx]+1;i <= size;i++) {
		if(n >= i) {
			sum += i;
			n -= i;
			a[idx] = i;
			decompose2(n,a,next,size,sum,idx+1,a[idx]);
			sum -= i;
			n += i;
		}
	}

	//printf("a[%d]:%d\n",idx,a[idx]);
	sum -= a[idx];
	n += a[idx];
	/*
	if(i == (size+1)) {
		--idx;
		sum -= a[idx];
		n += a[idx];
		next[idx] = a[idx];
	}
	*/
	return 0;
}

/*
 * n:剩余数值
 * visited:设置访问标志
 * sum:当前元素的总和
 * 当前记录数组可用的位置
 */
int decompose(int n,int a[],int visited[],int size,int sum,int idx,int current)
{
	int i;

	if(n == 0) {
		puts("decompose:");
		for(i = 0;i < idx;++i)
			printf("%d ",a[i]);
		printf("\n");
		count++;
		//printf("sum:%d,n:%d,idx:%d\n",sum,n,idx);
		//printf("idx:%d,%d\n",idx,a[idx]);
		//getchar();
		return 0;
	}
	
	//printf("begin loop:%d,idx:%d,sum:%d,n:%d,a[0]:%d\n",current,idx,sum,n,a[0]);
	//print(a,idx);
	for(i = current;i <= size;i++) {
		if(!visited[i] && n >= i && sum >= 0 && idx < size) {
			//printf("a[0]:%d,idx:%d,i:%d\n",a[0],idx,i);
			a[idx] = i;
			visited[i] = 1;
			sum += i;
			n -= i;
			//print(a,idx);
			//printf("idx:%d,i:%d,n:%d,sum:%d\n",idx,i,n,sum);
			break;
		}
	}
	//puts("--------end loop -----------");
	//not found i,clear idx
	if(i == size+1) {
		idx--;
		visited[a[idx]] = 0;
		n += a[idx];
		sum -= a[idx];
		//printf("not found,clear idx:%d,a[idx]:%d,sum:%d,n:%d\n",idx,a[idx],sum,n);
		return 0;
	}
	
	//choose i
	//printf("next recursion idx:%d,sum:%d,n:%d\n",idx+1,sum,n);
	decompose(n,a,visited,size,sum,idx+1,a[idx]+1);
	//dont' choose i
	visited[a[idx]] = 0;
	n += a[idx];
	sum -= a[idx];
	//clear i
	decompose(n,a,visited,size,sum,idx,a[idx]+1);
	
	return 0;
}


int main(int argc,char *argv[])
{
	int i,a[N],visited[N];
	for(i = 0;i < N;i++) {
		a[i] = 0;
		visited[i] = 0;
	}
	puts("decompose:");
	printf("0 %d\n",N);
	decompose2(N,a,visited,N,0,0,1);
	printf("count:%d\n",count);
	return 0;
}