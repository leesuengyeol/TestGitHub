//RX

#include <siganl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MSIZ 30

struct score{
	char name[20];
	int kook;
	int young;
	int su;
};

void sigHandler(int sig)
{
	printf("Tx : complete sotred");
	(void) signal(SIGINT ,SIG_DFL);
}

int main(void)
{
	(void)signal(SIGINT,sigHandler); //저장을 완료한 Tx 프로세서가 SIGINT를 보낼것이기때문에
	pause(); //시그널을 기다린다
 	
	int personNum=0;
	struct score person[30];
	struct score * cshmaddr;
	int shmgid;
	void* shmaddr = (void*) 0;
	int running =1;
	int avr;
	int sum;

	shmgid=shmget((key_t)1234, MSIZ*sizeof(person),0666|IPC_CREAT);
	
	if(shmgid ==-1)
	{
		fprintf(stderr,"shmget was failled\n");
		exit(EXIT_FAILURE);
	}

	shmaddr=shmat(shmgid, (void*)0 ,0);

	if(shmaddr ==(void*)-1)
	{
		fprintf(stderr,"shmat was failed\n");
		exit(EXIT_FAILURE);
	}	
	
	cshmaddr = (struct score*)shmaddr;
	
	while(running)
	{			
		person[personNum]=*(cshmaddr+personNum); //구조체 가져옴
		if(!strncmp(person[personNum].name,"end",3))
		{
			running =0;
		}
		else
		{
			sum = person[personNum].kook+person[personNum].young+person[personNum].su;
			avr = sum/3;
			printf("%s의 총점:%d 평균: %d\n" , person[personNum].name,sum,avr);	
			personNum++;
		}
	}
	if((shmdt(shmaddr)==-1))
	{
		fprintf(stderr,"shmdt was failled\n");
		exit(EXIT_FAILURE);
	}
	
	if(shmctl(shmgid,IPC_RMID,0) ==-1)
	{
		fprintf(stderr,"shmctl was failled");
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_SUCCESS);
}

