#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//gettimeof
#include <sys/time.h>
#include <time.h>

//ipc
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>

//#define DEBUG

struct threadV{
	int start;
	int end;
	long long result;
};

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
}arg;



int operateSS(int start, int end); // 싱글 쓰레드 동작
void* singleF(void *data);  //싱글 쓰레드 함수
int operateDS(int start, int end); //더블 쓰레드 동작
void* doubleF(void *data); //더블 쓰레드 함수

int main(int argc , char* argv[])
{
	int start=atoi(argv[2]);
	int end =atoi(argv[3]);

	if(argc == 0) //인자가 없을때
	{
		perror("chose thread amount, restart program \n");
		return -1;
	}
	else if(argc == 1||argc ==2)  //value 값을 안적었을때
	{
		perror("start value and end value, restart program \n");
		return -1;
	}
	else
	{
		if(!strcmp(argv[1] , "-S")) //싱글 스레드일때
		{
			operateSS(start,end);	
		}
		else if(!strcmp(argv[1],"-D")) //듀얼 스레드 일때
		{
			operateDS(start,end);
		}
		else //잘못 입력햇을때
		{
			perror("chose one -S or -P, restart program\n");
			return -1;
		}
	}
	
	return 0;
}

int operateDS(int start, int end)
{
	pthread_t p_thread[2]; //스레드 아이디
	int result;  //저장할 결과값
	int resultsum=0;
	struct threadV dt[2]={{start,end/20,0},{(end/20+1),end,0}}; //구조체 초기화

	pthread_create(&p_thread[0],NULL,doubleF,(void*)&dt[0]);
	pthread_create(&p_thread[1],NULL,doubleF,(void*)&dt[1]);

	pthread_join(p_thread[0],(void**)&result);
	resultsum += result;
	pthread_join(p_thread[1],(void**)&result);
	resultsum += result;

	printf("Double Thread Result : %d\n",resultsum);
	return 0;
}

void* doubleF(void *data)
{
	struct threadV dt = *((struct threadV*)data);
	int i;
	for(i=dt.start;i<=dt.end;i++)
	{
		dt.result += i;
	}

	return (void *)dt.result ;
}


int operateSS(int start, int end)
{
	pthread_t p_thread; //스레드 아이디
	int result;  //저장할 결과값
	struct threadV st= {start,end,0}; //구조체 초기화
	pthread_create(&p_thread,NULL,singleF,(void *)&st);  //스레드 생성
	pthread_join(p_thread,(void **)&result); //스레드 동작후 값 반환 
	//주소값을 넘겨주어야 참조에 의한 값저장이 가능하기 때문에 주소를 넘겨주고 더블포인터를 ㄹ사용한다
	printf("Single Thread Result : %d\n",result); //반환된값 출력
	return 0;
}

void* singleF(void *data)  //싱글 쓰레드
{
	struct threadV st= (*(struct threadV *)data);
	int i;

	for(i=st.start; i<=st.end;i++)
	{
		st.result += i;
	}
#ifdef DEBUG
	printf("st.result: %lld , (void*)st.result :%p,&st.result: %p  \n",st.result,(void *)st.result);
	fflush(stdout);
#endif
	return (void *)st.result;
}
