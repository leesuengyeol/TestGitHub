#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("Running ps with systemp\n");
	system("ps -ax &");
	printf("Done.\n");

	exit(0);
}
