#include "mysum.h"
#include <stdio.h>


int main(void)
{
		int a;
		int b;

		scanf("%d %d",&a,&b);
		printf("%d + %d = %d\n",a,b,ysum(a,b));
		return 0;
}
