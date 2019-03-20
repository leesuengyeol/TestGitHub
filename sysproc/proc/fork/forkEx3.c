#include <stdio.h>
#include <sys/types.h> //pid_t
#include <unistd.h>//STDOUT_FILENO
#include <stdlib.h>//exit()


//external variable in initialized data
int glob =6;
char buf[] = "a write to stdout\n";

int main(void)
{
		//automatic variable on the stack

		int var;
		pid_t pid;
		var =88;

		if(write(STDOUT_FILENO,buf,sizeof(buf)-1) !=sizeof(buf)-1)  //write 는 정상적으로 썻다면 쓴만큼의 bite를 반환함
				perror("write error");

		printf("before fork\n"); //we don't flush stdout
		
		//자식 프로세서 생성 & 자식 프로세스인 경우
		if ((pid =fork())==0) 
		{
			glob++;// 자식프로세스만 값 증가
			var++;
		}
		else { //부모 프로세스인경우
			wait();
				//	sleep(2);//자식이 동작하고 부모가 동작하도록
		}

		printf("pid = %d,glob =%d,var =%d\n",getpid(),glob,var);
		exit(0);
}
