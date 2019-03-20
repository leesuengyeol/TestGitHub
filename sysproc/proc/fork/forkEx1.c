#include<stdio.h> //printf()
#include <unistd.h>  //fork()
#include <sys/types.h>//pid_t


int main(void)
{
	pid_t pid;
	int x=0;
	int status;


	x=1;
	if ((pid = fork())==0)
	{
		printf("pid =%ld,I am process %ld and getppid = %ld\n",(long)pid,(long)getpid(),(long)getppid());
		sleep(1);
		printf("Done\n");
		return 0;
	}
	else
	{
		printf("pid =%ld,I am process %ld \n",(long)pid,(long)getpid());
	}
		wait(&status); //자식프로세서가 끝나는것을 기다림
	
	return 0;
}	

