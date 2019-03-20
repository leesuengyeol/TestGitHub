#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void sig_quit(int);

int main(void)
{
	sigset_t newmask,oldmask ,pendmask;

	if(signal(SIGQUIT,sig_quit) == SIG_ERR)
			perror("can't catch SIGQUIT");

	sigemptyset(&newmask);//배열 모두 0으로

	sigaddset(&newmask,SIGQUIT); //newmask의 SIGQUIT을 1로 만듬

	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0) //newmask비트를 oldmask로 백업한후 newmask 1을 block함 //sigprocmask가 0보다 작으면 실패로 에러 출력
			perror("SIG_BLOCK error");

	sleep(5);

	if(sigpending(&pendmask)<0) //sigprocmask와 비교하기위해 만듬 (sigpending)
			perror("sigpending error");

	if(sigismember(&pendmask , SIGQUIT))  //SIGQUIT의 비트가 1인지 아닌지 (1이면 참)
			printf("\nSIGQUIT pending\n");


	if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)  //oldmask로 다시 세팅
			perror("SIG_SETMASK error");

	sleep(5);
	exit(0);
}

static void sig_quit(int signo)
{
	printf("caught SIGQUIT\n");
	if(signal(SIGQUIT,SIG_DFL) ==SIG_ERR)
			perror("can't reset SIGQUIT");
}
