#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

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
	wait(&status); //return은 종료된 자식프로세스 ID status 는 자식프로세스의 상태를 저장

					//왜 status가 필요한가????
	exit(0);
}
