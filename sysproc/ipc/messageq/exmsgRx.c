//Rx
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> //use errno
#include <unistd.h>
#include "exmymsg.h"

//for IPC function
#include <sys/types.h>
#include <sys/ipc.h> //key_t
#include <sys/msg.h> //msgget msgsnd msgrcv msgctl
	
int main(void)
{

	int running =1;
	struct myData person[10];
	struct msqid_ds msqstat; //메시지큐의 남은 메시지 갯수를 확인하기 위해
	int msgid;
	char buffer[BUFSIZ];
	int personNum =0;
	
	//step0. ftok() ->보안을 위한 key값 생성

	//step1. msgget
	msgid= msgget((key_t)1234, 0666|IPC_CREAT);
	if (msgid==-1)
	{
		fprintf(stderr,"Erroar: msgget failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}


	while(running)
	{
		//step2. msgrcv
		if((msgrcv(msgid, &person[personNum],sizeof(person) -sizeof(long),0,0)==0))
		{
			fprintf(stderr,"Erroar: msgget failed : %d\n", errno);
			exit(EXIT_FAILURE);	
		}
		if(person[personNum].msgType==2)
				running=0;
		if(strcmp(person[personNum].name,"end")==0)
		{
			break;
		}
		else
		{
		printf("Receive Data Person[%d]\n\n" , personNum);
		printf("Name:%s\n",person[personNum].name);
		printf("총점:%d\n",person[personNum].kook+person[personNum].young+person[personNum].su);
		printf("평균:%f\n",(person[personNum].young+person[personNum].kook+person[personNum].su)/3.0);
		}
		
	/*	if(msgctl(msgid ,IPC_STAT,&msqstat)==-1)
		{
			perror("FAIL : msgctl()");
			exit(1);
		}
		printf("\n\n\nremain message count: %ld\n",msqstat.msg_qnum);
		
		if(msqstat.msg_qnum>0)
		{
				printf("Next Person information\n");
				running =1;
		}*/
		personNum++;
	}
	//step3 .msgctl IPC_RMID
	if(msgctl(msgid, IPC_RMID,0) ==-1)
		{
			fprintf(stderr,"Erroar: msgget failed : %d\n", errno);
			exit(EXIT_FAILURE);	
		}
	exit(EXIT_SUCCESS);
}
