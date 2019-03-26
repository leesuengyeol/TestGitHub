//과제1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

//POSIX Thread

#include <pthread.h>

struct value{
	int start;
	int end;
	long long result;
};




void* t_function1(void* data){
	int i;
	struct value value = (*(struct value*)data);

	for(i=value.start; i<=value.end; i++)
	{
		value.result +=i;
	}
	
	return (void *)value.result;
}


/*void* t_function2(void* data)
{
	int i;
	long long result=0;
	for(i=1000001;i<*((int*)data)+1;i++)
	{
		result +=i;
	}

	return (void *)result;
}*/

struct timeval calCT(struct timeval start, struct timeval end)
{
	struct timeval diff;
	if(start.tv_usec > end.tv_usec)
	{
		end.tv_sec--;
		diff.tv_usec = 1000000 + end.tv_usec -start.tv_usec;
		diff.tv_sec = end.tv_sec - start.tv_sec;
	}
	else 
	{
		diff.tv_usec = end.tv_usec -start.tv_usec ;
		diff.tv_sec =  end.tv_sec -start.tv_sec;
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
	pthread_t p_thread[2];
	int ret;
	struct value fev={1,1000000,0};
	struct value sev={1000001,2000000,0};

	long long status;
	long long result =0;
	if(ret= pthread_create(&p_thread[0],NULL,t_function1,(void *) &fev)<0) //NULL 자리는 쓰레드의 특성을 담을 변수
	{
		perror("Error:pthread_creat1()");
		return -1;
	}
	
	if(ret= pthread_create(&p_thread[1],NULL,t_function1,(void *) &sev)<0) //NULL 자리는 쓰레드의 특성을 담을 변수
	{
		perror("Error:pthread_creat1()");
		return -1;
	}

	pthread_join(p_thread[0],(void**)&status);
	result += status;
	
	pthread_join(p_thread[1],(void**)&status);
	result += status;
	
	printf("dual thread :    %lld\n",result);
	
	gettimeofday(&end,NULL); //end time
	diff = calCT(start,end);
	printf("dual thread time: %ld.%ld\n",diff.tv_sec,diff.tv_usec);
}
