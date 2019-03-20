#include <signal.h>
#include <stdio.h>
#include <unistd.h>


//SIGINT에 대응하는 핸들러
void sigHandler(int sig)
{
		static count =0;
		printf("OUCH! - i got signal %d\n", sig);
		if(count==){
	//	(void) signal(SIGINT, SIG_DFL); //sigint를 원래 처리하던 방식으로 처리

		(void) signal(SIGINT, SIG_IGN); 
		}
		count++;
}

//main함수
int main()
{
		//SIGINT 신호가 수신되었을 때
		//sigHandler() 함수가 호출되도록 등록
		signal(SIGINT, sigHandler);

		while(1){
				printf("Hello World!\n");
				sleep(1);
		}
}
