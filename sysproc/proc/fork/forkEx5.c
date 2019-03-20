#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static void DisplayChar(char *str)
{
		char *ptr; int c;

		//stdout 의 버퍼링을 막는다
		setbuf(stdout, NULL);
		for(ptr = str; c= *ptr++;)
				putc(c,stdout);
}


int main(void)
{
		pid_t pid;
		if((pid=fork())<0)
				perror("fork error");
		else if (pid==0)
				DisplayChar ("output from child\n");
		else
				DisplayChar("output from parent\n");
		exit(0);
}
