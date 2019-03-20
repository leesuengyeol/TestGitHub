#include <stdio.h>
#include <unistd.h>

int main(void)
{
	/* execv
	//"파일명","인자","인자끝(NULL) 표시 배열
	char *argv[] = {"ls","-a",0};

	//execv("경로",인자 배열
	execv("/bin/ls",argv);
	*/

	//execvp execlp
	char *argv[] ={"ls","-al","\tmp",NULL};

	execlp("ls","ls","-al","\tmp",NULL);
	
	execvp("ls",argv);
	
	printf("이 메시지가 보이면 지정된 프로그램이 \ 없거나어떤문제로 실행되지 못한 것입니다.\n");

}
