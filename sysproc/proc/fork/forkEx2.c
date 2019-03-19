//자식프로세서 만들기

//wait없이 부모와 자식 프로세스 어떤게 먼저 실행될것인가?


#include <stdio.h>//printf
#include <sys/types.h> //pid = pid_t
#include <unistd.h> //fork()

int global =10;

int main(void)
{
	int i=20;
	pid_t pid;
	int status;
	pid = fork();
	//child process
	if(pid ==0)
	{
		global = global+10;
		i=i+10;
	}
	//parent process
	else
	{
		global = global +100;
		i = i+100;
	}
	printf("global = %d; i= %d\n",global,i);
}


//랜덤으로 실행됨(어떤 프로세스가 먼저 실행될지 알수가 없다)
