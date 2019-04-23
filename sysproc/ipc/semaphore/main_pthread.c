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

long long temp;

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

//세마포어
static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);


//쓰레드
int operateSS(int start, int end); // 싱글 쓰레드 동작
void* singleF(void *data);  //싱글 쓰레드 함수
int operateDS(int start, int end); //더블 쓰레드 동작
void* doubleF(void *data); //더블 쓰레드 함수

//프로그램 실행 시간 함수
struct timeval calCT(struct timeval start, struct timeval end);

static int sem_id;

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
	struct timeval timestart,timeend,timediff;
	gettimeofday(&timestart,NULL); //start time

	pthread_t p_thread[2]; //스레드 아이디
	long long result;  //저장할 결과값
	long long resultsum=0;
	struct threadV dt[2]={{start,end/20,0},{(end/20+1),end,0}}; //구조체 초기화
	
	if((sem_id= semget(IPC_PRIVATE,1,IPC_CREAT|0666))==-1)
	{
		perror("Error:semget()");
		return -1;
	}

	arg.val=1;
	
	set_semvalue();



	pthread_create(&p_thread[0],NULL,doubleF,(void*)&dt[0]);
	pthread_create(&p_thread[1],NULL,doubleF,(void*)&dt[1]);

	pthread_join(p_thread[0],(void**)&result);
	pthread_join(p_thread[1],(void**)&result);

	printf("Double Thread Result : %lld\n",temp);
	del_semvalue();
	//시간출력
	gettimeofday(&timeend,NULL); //end time
	timediff = calCT(timestart,timeend);
	printf("dual thread time : %ld.%ld\n",timediff.tv_sec,timediff.tv_usec);
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

	semaphore_p();
	temp += dt.result ;
	semaphore_v();

	return (void *)dt.result ;
}


int operateSS(int start, int end)
{
	struct timeval timestart,timeend,timediff;
	gettimeofday(&timestart,NULL); //start time

	pthread_t p_thread; //스레드 아이디
	long long result;  //저장할 결과값
	struct threadV st= {start,end,0}; //구조체 초기화
	pthread_create(&p_thread,NULL,singleF,(void *)&st);  //스레드 생성
	pthread_join(p_thread,(void **)&result); //스레드 동작후 값 반환 
	//주소값을 넘겨주어야 참조에 의한 값저장이 가능하기 때문에 주소를 넘겨주고 더블포인터를 사용한다
	printf("Single Thread Result : %lld\n",result); //반환된값 출력
	//시간출력
	gettimeofday(&timeend,NULL); //end time
	timediff = calCT(timestart,timeend);
	printf("dual thread time : %ld.%ld\n",timediff.tv_sec,timediff.tv_usec);
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
	printf("st.result: %lld , (void*)st.result :%d  \n",st.result,(void *)st.result);
	fflush(stdout);
#endif
	return (void *)st.result;

}

static int set_semvalue(void)
{
	union semun sem_union;

	sem_union.val =1;
	if(semctl(sem_id,0,SETVAL,sem_union)==-1) return 0;

	return (1);
}

static void del_semvalue(void)
{
	union semun sem_union;

	if(semctl(sem_id,0,IPC_RMID,sem_union)==-1)
			fprintf(stderr,"Failed to delete semaphore\n");
}

static int semaphore_p(void)
{
	struct sembuf sem_b;

	sem_b.sem_num=0;
	sem_b.sem_op =-1; //p()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id,&sem_b,1)==-1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return (0);
	}
	return (1);
}

static int semaphore_v(void)
{
	struct sembuf sem_b;

	sem_b.sem_num=0;
	sem_b.sem_op =1; //V()
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id,&sem_b,1) ==-1)
	{
		fprintf(stderr,"semaphore_v failed\n");
		return (0);
	}
	return (1);
}

struct timeval calCT(struct timeval start, struct timeval end)
{   
     struct timeval diff;
     if(start.tv_usec > end.tv_usec)
     {
         end.tv_sec--;
          diff.tv_usec = 1000000 + end.tv_usec -start.tv_usec;
          diff.tv_sec= end.tv_sec - start.tv_sec;
     }     
	 else
     {
         diff.tv_usec = end.tv_usec -start.tv_usec ;
         diff.tv_sec =  end.tv_sec -start.tv_sec;
     }
      
     return diff;
}

