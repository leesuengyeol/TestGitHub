//좀비프로세서 확인!!
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

#define MYTIME 20

int main()
{
	pid_t pid;
	char *message;
	int n;
	int status;

	printf("fork program starting\n");
	pid = fork();

	switch(pid)
	{
			case -1:	perror("fork failled");
						exit(1);
			case 0:		message = "This is the child";
						n=5;
						break;
			default:	message = "This is the parent";
						n=3;
						break;
	}

	for(;n>0; n--){
			puts(message);
			sleep(1);
	}
	if(pid>0)
	{
			printf("CHild Proc State : Zombie, about %d sec\n",MYTIME);
			sleep(MYTIME);
			wait(NULL);
	}
	exit(0);

}
