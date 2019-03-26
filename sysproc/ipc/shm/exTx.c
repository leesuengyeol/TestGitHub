//Tx

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//for ipc

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

int main(void)
{
	int shgid; //공유메모리 아이디
	void *shaddr = (void*)0;  //공유 메모리 주소 저장공간
	struct score *cshaddr;//주소를 증가시키면서 데이터를 저장해야 하기때문에 shaddr 로부터 int형으로 변환시킬 변수
	int i; //반복문 사용
	struct score person[30];
	int running=1;
	int personNum=0;

	if((shgid=shmget((key_t)1234,MSIZ * sizeof(person),0666|IPC_CREAT))==-1) //공유메모리를 만들어 id를 받아온다
	{
		fprintf(stderr,"shmget was failled\n");
		exit(EXIT_FAILURE);
	}
	
	if((shaddr = shmat(shgid, (void*)0,0)) ==(void *)-1) //만들어진 공유메모리에 프로세스를 attach하여 메모리 주소를 받아온다
	{
		fprintf(stderr,"shmat was failled\n");
		exit(EXIT_FAILURE);
	}

	cshaddr =(struct score *) shaddr; //주소를 받는다

	while(running)
	{
		printf("Name: ");
		scanf("%s",person[personNum].name);
		if(!strncmp(person[personNum].name,"end",3))
		{
			running = 0;
		}
		else 
		{
			printf("kook: ");
			scanf("%d",&person[personNum].kook);
			while(getchar() !='\n');
			printf("young: ");
			scanf("%d",&person[personNum].young);
			while(getchar() !='\n');
			printf("su: ");
			scanf("%d",&person[personNum].su);
			while(getchar() !='\n');
		}	
		
		*(cshaddr+personNum) =person[personNum] ; //주소를 증가시키며 1~100까지 집어넣는다
		personNum++;
	}

	if((shmdt(shaddr)==-1)) //프로세스를  공유메모리로부터 dettach 한다.
	{
		fprintf(stderr,"shmdt was failled\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS); //성공적인 프로세스 종료
}
