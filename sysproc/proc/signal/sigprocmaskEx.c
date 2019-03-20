#include <stdio.h> 	//printf
#include <signal.h> // SIGUSR
#include <unistd.h> // sleep
#include <wait.h>  	//wait
#include <stdlib.h>	//exit
#include <sys/types.h>	//pid_t



void pr_exit(int status)
{
		//정상 종료인경우
		if(WIFEXITED(status))
				printf("normal termination, exit status = %d\n", WEXITSTATUS(status));

		//시그널에 의해서 종료된 경우
		else if (WIFSIGNALED(status))
				printf("abnormal termination ,signal number = %d%s\n", WTERMSIG(status), WCOREDUMP(status)?"(core file generated)" : "");
	
		//시그널에 의해 stop된 경우
		else if (WIFSTOPPED(status))
				printf("child stopped, signal number =%d\n" ,WSTOPSIG(status));
}

void sighandler(int sig) //sig에 따른 출력함수
{
	if(sig == SIGUSR1)
			printf("Hello\n");
	else//(sig == SIGUSER2)
			printf("World\n");
}
								

int main(void)
{
	int count=0;
	int status;
	pid_t pid;
	sigset_t newmask, oldmask, pendmask;
	pid = fork();
	if(pid == -1)  //fork 안됬을때 에러 메세지 출력
		{
			printf("Error: fork pid = -1");
			return -1;
		}
	else if(pid==0) //자식 프로세서일때
	{

		if(signal(SIGUSR1,sighandler)==SIG_ERR)
			printf("\ncan't catch SIGUSR1\n");
		if(signal(SIGUSR2,sighandler)==SIG_ERR)
			printf("\ncan't catch SIGUSR1\n");
		
		//SIGUSR1 SIGUSR2 blocking
		sigemptyset(&newmask);
		sigaddset(&newmask , SIGUSR1);
		sigaddset(&newmask,SIGUSR2);

		if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0) // sigprocmask가 0보다 작으면 SIG_BLOCK이 제대로 안된것
				perror("SIG_BLOCK error\n");
		
		if(sigpending(&pendmask)<0)
				perror("sigpending error");

		if(!sigismember(&pendmask,SIGUSR1))
				printf("\nSIGUSR1,SIGUSR2 are blocking\n");

		sleep(6); //5초대기 후 unblocking

		//SIGUSR1 SIGUSR2 unblocking
		if(sigprocmask(SIG_SETMASK, &oldmask,NULL)<0) //sigprocmask를 블록하기 전으로 돌림
		{
				printf("SIG_SETMASK error\n");
				return -1;
		}

		printf("SIGUSR1 , SIGUSR2 are unblocking\n");
		
		while(1)
		pause();  //signal 이 들어올때까지 대기
	}

	else//부모프로세서일때 
	{
		sleep(1);
		while(count<5)
		{
			kill(pid,SIGUSR1); //SIGUSR1 자식프로세서에 전송
			sleep(1);
			kill(pid,SIGUSR2); //SIGUSR2 자식프로세서에 전송
			sleep(1);
			count++;	
		}
		kill(pid,SIGKILL);
	}
	wait(&status);
	pr_exit(status);
	return 0;
}
