//안됨 semex.c 참고할것

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//IPC & semaphore
#include <sys/ipc.h> 
#include <sys/sem.h>




union semun //<union>	
			//가장 데이터 타입이 큰거만큼 구조체의 크기를 정함
			//선언한 변수 모두 사용가능하지만 동시에 사용은 할수 없다
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
}arg;

int cnt =0;
static int semid;
/*struct sembuf *sops	
세마포어 값을 계산하기위한 설정 값
struct sembuf {
    short sem_num;   세마포어 번호
    short sem_op;     세마포어 증감값
    short sem_flg;     옵션 
flag
1. IPC_NOWAIT -> 진입하지못할때 바로복귀 (대기없음)
2. SEM_UNDO   -> 프로세스가 종료되면 시스템에서 세마포어 설정을 원래 상태로 되돌림 (보통 이옵션 사용!!)
}*/

void p() //semaphore p (critical section 진입할때)
{
	struct sembuf pbuf;
	pbuf.sem_num=0;
	pbuf.sem_op =-1;
	pbuf.sem_flg = SEM_UNDO;
	if(semop(semid,&pbuf,1)==-1)//critical section 진입가능한지 확인 불가능하다면 계속대기 (SEM_UNDO)
								//1(세번째 인자): 변경하려는 세마포어의 개수 (여러개일때 사용)
		perror("p:semop()");
}


void v() //semaphore v (critical section 빠져나올때)
{
	struct sembuf vbuf;
	vbuf.sem_num=0;
	vbuf.sem_op =1;
	vbuf.sem_flg = SEM_UNDO;
	if(semop(semid,&vbuf,1)==-1) //1(세번째 인자): 변경하려는 세마포어의 개수 (여러개일때 사용)
		perror("v:semop()");
}

int main(int argc, char *argv[])
{
	//step1. semget
	if ((semid=semget(IPC_PRIVATE,1,IPC_CREAT|0666))==-1) //private 이 프로세서 안에서만 사용(일단 테스트용)
													//두번째 인자는 세마포어를 만들지 이미 있는 세마포어에  접근할지
	{
		perror("Error:semget()");
		return -1;
	}

	arg.val=1;

	//step2. semctl SETVALUE
	if (semctl(semid,0,SETVAL,arg)==-1); //SETVAL값을 정
	{
		perror("Error:semctl():SETVAL");
		return -1;
	}

	//TEST CODE
	while(1)
	{
		if(cnt<9)
		{
			cnt++;
			v();
			printf("increase : %d\n",cnt);
			usleep(100);
		}
		else
		{
			p();
			cnt--;
			printf("decrease : %d\n",cnt);
			break;
		}
	}

	//step3. semctl IPC_RMID
	if (semctl(semid,0,IPC_RMID,arg) ==-1)
	{
		perror("semctl():IPC_RMID");
	}

	return 0;
}

