// Calculator.cpp : Defines the entry point for the console application.
//
/*
 * 计算器求值 运算符仅支持:+ - * ( )
 * 
 */
//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
/*
 * 定义优先级数组
 * 
 * l/r   +   -   *   (   )
 * +     =   =   <   <   >       
 * -     =   =   <   <   >
 * *     >   >   =   <   >
 * (     >   >   >   <   =
 * )     <   <   <   <   >
 */
//=:0 <:1 >:2
static int prority[5][5] = {
	{0,0,1,1,2},//+
	{0,0,1,1,2},//-
	{2,2,0,1,2},//*
	{2,2,2,1,0},//(
	{1,1,1,1,2}
};

static int get_priority(int left,int right)
{
	switch(left) {
	case '+': { 
			switch(right) {
				case '+': return 0;
				case '-': return 0;
				case '*':
				case '(': return 1;
				case ')': return 2;
			  }
			  } break;
	case '-':{
		switch(right) {
		case '+':
		case '-': return 0;
		case '*': 
		case '(': return 1;
		case ')': return 2;
		}
			 } break;
	case '*': {
		switch(right) {
		case '+':
		case '-': return 2;
		case '*': return 0;
		case '(': return 1;
		case ')': return 2;
		}
			  } break;
	case '(': {
		switch(right) {
		case '+':
		case '-':
		case '*': return 2;
		case '(': return 1;
		case ')': return 0;
		}
			  } break;
	case ')': {
		switch(right) {
		case '+':
		case '-':
		case '(':
		case '*': return 1;
		case ')':return 2;
		}
			  }
	default: break;
	}

	return -1;
}

int calculate(int left,char op,int right)
{
    printf("left:%-d,right:%-d,op:%c\n",left,right,op);
    
	switch(op) {
	case '+': return left + right;
	case '-': return left - right;
	case '*': return left * right;
	default: assert(1);
	}

	return -1;
}

int next_op(const char *q,int *value,int *length)
{
	char str[20];
	int i = 0;
	const char *p = q;

	if(*p == '-' || *p == '+') str[i++] = *p++;
	
	//puts(p);

	while(p !='\0' && isdigit(*p)) {
		str[i++] = *p++;
	}
	
	str[i] = '\0';
	*value = atoi(str);
	if(*p == '\0') {
		*length = p - q;
		return -1;
	}
	*length = p - q + 1;
	
	return *p;
}
/*
 * 获取第一个匹配的'('/')'
 */
const char* __get_last_token(const char *p,int size) 
{
    int count = 0;
    
    while(*p != '\0' && *p != '(')
        p++;
    
    if(*p == '\0') {
        fprintf(stderr,"illegal expression.\n");
        exit(-1);
    }
    
    count++;
    p++;
    
    while(*p != '\0' && count > 0) {
        if(*p == ')')
            count--;
        else if(*p == '(')
            count++;
        p++;
    }
     
    if(count == 0)
        return p;

    return NULL;
}
/*
  * 计算*的连乘表达式
  */
static int __calculate_num(const char *p,int size,int *len)
{
    const char *q;
    char str[size+1];
    int i,left,right,op;
    
    memset(str,0,size+1);
    q = p;
    
    if(*q == '\0' || !size)
        return 1;
    
    if(*q == '+' || *q == '-') {
        str[i++] = *q++;
    }
    
    while(*q == '\0' && isdigit(*q)) {
        str[i++] = *q++;
    }
    
    str[i++] = '\0';
    
    left = atoi(str);
    if((op = next_op(q,&right,&i)) < 0)
        return left;
    
    left = calculate(left,op,right);
    
    return calculate(__calculate_num(p+i,size - i,&len),'*',size - p -i);
}


int __recursive_calculator(const char *p,int size)
{
	const char *q = p;
	char str[20];
	int i;
	/*
	 * left:左边值,right:右边值,lop:当前操作符,rop:下一个操作符
	 */
	int left,right,length;
	char lop,rop;
	if(*p == '\0' || size == 0)
		return 0;
	i = 0;

    //check ( in here
    if(*q == '(') {
        
        if(!(q = __get_last_token(q,size))) {
            fprintf(stderr,"illegal expression\n");
            exit(-1);
        }
        
        left = __recursive_calculator(p+1,q - p - 2);
        
        if(*q != '*')
            return calculate(left,'+',\
                             __recursive_calculator(q,size - (q - p)));
        else {
            /*
             * 这里需要判断到直到优先级小于*为止
             */
            memset(str,0,20);
            
            while(*q != '\0' && isdigit(*q))
                str[i++] = *q++;
            
            str[i++] = '\0';
            right = atoi(str);
            left = calculate(left,'*',right);
            
            return calculate(left,*q,\
                             __recursive_calculator(q+1,size-(q+1-p)));
        }
    }
    
    memset(str,0,20);
    
	if(*q == '+' || *q == '-') 
		str[i++] = *q++;

	while(*q != '\0' && isdigit(*q))
			str[i++] = *q++;

	str[i++] = '\0';
	left = atoi(str);
    //only one operator,stop recursion
    if(*q == '\0')
        return left;
    
	lop = *q++;
    //only one operator
	if((rop = next_op(q,&right,&length)) < 0)
		return calculate(left,lop,right);
	//q 指向右边表达式
	switch(get_priority(lop,rop)) {
	case 0: // lop == rop //从左到右进行计算
		{
			left = calculate(left,lop,right);
            printf("right:%s,size:%d\n",q+length,(size - (q - p +length)));
			return calculate(left,rop,__recursive_calculator(q+length,size - (q - p + length)));
		} break;
	case 1: //lop < rop,右边优先级高先算右边表达式的值，再计算左边
        /*
         * 也可以不需要,直接加上符号位就可以
         */
        printf("left operand less than right operand,str:%s\n",q);
        //return calculate(left,lop,__recursive_calculator(q,size - (q-p)));
        return calculate(left,'+',__recursive_calculator(q - 1,1 + size - (q-p)));
		break;
	case 2: //lop > rop //从左到右进行计算
		{
            printf("left operand greater than right operand,str:%s\n",q+length-1);
			left = calculate(left,lop,right);
            //将第一个符号位加入到后面的表达式中去
            return calculate(left,'+',__recursive_calculator(q + length - 1,1 + size - (q - p + length)));
			//return calculate(left,rop,__recursive_calculator(q+length,size - (q - p + length)));
		} break;
	default: assert(1);break;
	}
	

	return 0;
}

void calculator(char *input,int *res)
{
	*res = __recursive_calculator(input,strlen(input));
}

int main(int argc, char* argv[])
{
	int sum;

	if(argc != 2) {
		fprintf(stderr,"usage:%s expression.\n",argv[0]);
		return -1;
	}
	calculator(argv[1],&sum);
	printf("expression:%s,value:%d\n",argv[1],sum);

	return 0;
}
