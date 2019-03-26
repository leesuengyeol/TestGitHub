//안됨
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//for IPC
#include <sys/types.h>
#include <sys/ipc.h> //key_t
#include <sys/shm.h> //shmget, shmat ,shmdt , shmctl

#define SHMSIZE 100  //실질적으로 int * 100

int main(void)
{
	void *shared_Mem = (void*) 0 ; //null 포인터로 아직메모리가 할당되지 않았다는것을 명시해 줌으로써 잘못된 메모리 접근을 막는다(그냥하면 쓰레기값이 들어가서 잘못된 메모리접근가능성있다)
	int shmid; //공유메모리 아이디
	int *shmaddr; //공유메모리 주소
	int i;

	//step0. ftok()

	//step1. shmget
	shmid = shmget((key_t)1234,sizeof(int)*SHMSIZE,0666|IPC_CREAT);
	if(shmid ==-1)
	{
		fprintf(stderr,"shmget failed\n");
		exit(EXIT_FAILURE);
	}

	//step2. shmat
	shared_Mem = shmat(shmid, (void*)0,0); //shmget 에서 받아온 id값 ,공유메모리 주소, flag =0)
	if(shared_Mem == (void*) -1)
	{
		fprintf(stderr,"shmat failed\n");
		exit(EXIT_FAILURE);	
	}

	printf("Memory attached at %X\n",(int)shared_Mem); //메모리 타입이기때문에 (int)로 강제 형변환
	shmaddr = (int*) shared_Mem; //이제부터 메모리 증가가능  


	//step3. memory access
	for(i=0; i<SHMSIZE;i++)
	{
		*(shmaddr+i) = i+1;
		printf("shmaddr: %X, data:%d\n", shmaddr+i, *(shmaddr+i));
	}

	//sleep(4);
	//step4. shmdt
	if(shmdt(shared_Mem)==-1)
	{
		fprintf(stderr,"shmdt failed\n");
		exit(EXIT_FAILURE);	
		
	}
	/*
	//step5. shmctl
	if(shmctl(shmid,IPC_RMID, 0)==-1)
	{
		fprintf(stderr,"shmctl (IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}*/
	exit(EXIT_SUCCESS);
}
