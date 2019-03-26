//과제1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

//POSIX Thread

#include <pthread.h>

void* t_function(void* data)
{
	int i;
	long long result=0;
	for(i=1;i<*((int*)data)+1;i++)
	{
		result +=i;
	}

	return (void *)result;
}

struct timeval calCT(struct timeval start,struct timeval end)
{
	struct timeval diff;

	if(start.tv_usec > end.tv_usec)
	{
		//올림수 해줘야함
		end.tv_sec--;
		diff.tv_usec = 1000000+end.tv_usec - start.tv_usec;
		diff.tv_sec = end.tv_sec - start.tv_sec;
	}
	else // 올림수 필요없을때
	{
		diff.tv_usec = end.tv_usec - start.tv_usec;
		diff.tv_sec = end.tv_sec - start.tv_sec;
	}

	return diff;
}

int main(void)
{
	//time
	int i,j;
	struct timeval start,end,diff;

	gettimeofday(&start,NULL); //start time
	//POSIX thread id data type
	pthread_t p_thread;
	int ret;
	int endv= 2000000;
	long long status;
	
	if(ret= pthread_create(&p_thread,NULL,t_function,(void *) &endv)<0) //NULL 자리는 쓰레드의 특성을 담을 변수
	{
		perror("Error:pthread_creat1()");
		return -1;
	}
	
	pthread_join(p_thread,(void**)&status);
	printf("sigle thread : %lld\n",status);
	
	gettimeofday(&end,NULL);

	diff=calCT(start,end); //end time
	printf("single thread time: %ld.%ld\n",diff.tv_sec,diff.tv_usec);
}
