//Tx
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
		printf("Name: ");
		scanf("%s",person[personNum].name);
	
		//person.name 에 end가 입력된다면 무한루프 탈출!(msg 전달 종료)
		if(	strncmp(person[personNum].name,"end",3) ==0) 
		{
			person[personNum].msgType =2;
			running =0;	
		}
		
		else
		{
		printf("kook: ");
		scanf(" %d",&person[personNum].kook);
		printf("young: ");
		scanf(" %d",&person[personNum].young);
		printf("su: ");
		scanf(" %d",&person[personNum].su);

		person[personNum].msgType =1;
		}

		//입력 버퍼 비우기 scanf가 개행문자 (enter)를 인식하기 때문에 비워줘야 한다
		while(getchar()!='\n');

		//step2. msgsnd
		
			if(msgsnd(msgid, &person[personNum],sizeof(person) - sizeof(long),0)==-1)
			{
				//메시지가 정상적으로 전달되지 않은 경우
				fprintf(stderr, "Error: msgsnd failed : %d \n",errno);
				exit(EXIT_FAILURE);
			}
			personNum++;
	}
}
