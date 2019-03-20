#include <signal.h> //signal()
#include <stdio.h>  //printf()
#include <stdlib.h> //exit()
#include <unistd.h> // pause()

static void sigHandler(int sig)
{
		static int count =0;
		count++;
		if(sig ==SIGINT)
		{
			
				printf("Caught SIGINT (%d)\n",count);
				return; //원래 프로그램상태로 돌아옴(이함수만 종료)  -main 문일경우 프로그램종료(= exit)
		}
		else  //if(sig ==SIGQUIT)
		{
			printf("Caught SIGQUIT (%d)\n",count);
		//	return;
		}
		printf("Caught SIGQUIT - that's all folks!\n");
		exit(0); //프로그램 종료
}

int main(int argc, char *argv[])
{
		if(signal(SIGINT,sigHandler) ==SIG_ERR) //signal 이 정상적으로 등록되지 않을경우 return -1
				return -1;

		if(signal(SIGQUIT, sigHandler) ==SIG_ERR)
				return -1;

		for(;;)
				pause(); //어떤 signal 이 수신될때까지 대기
}
