#include <stdio.h>

union t{
	int i;
	char c;
};

void main(void)
{
	union t t1;
	//t1.c[0] = 10;
    t1.i = 1;
    
	printf("%c\n",t1.c);
}
