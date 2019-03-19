#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void pr_exit(int status)
{
		//정상 종료인 경우
		if(WIFEXITED(status))
				printf("normal termination, exit status =%d\n", WEXITSTATUS(status));

		//시그널에 의해서 종료된 경우
		else if (WIFSIGNALED(status))
				printf("abnormal termination, signal number = %d%s\n",WTERMSIG(status),WCOREDUMP(status) ? "(core file generated)" : "");

		//시그널에 의해 STOP된 경우
		else if (WIFSTOPPED(status))
				printf("child stopped, signal number = %d\n",WSTOPSIG(status));

}

int main(void)
{
		pid_t pid;
		char *message;
		int n;
		int exit_code;

		printf("fork progranm strarting\n");

		pid = fork();
		switch(pid)
		{
				case -1: perror("fork failed");
						 exit(1);

				case 0: message = "This is the child";
						n=5;
						exit_code =37;
						break;
				
				default: message = "This is the parrent";
						 n=3;
						 exit_code =0;
						 break;
		}

		for(;n>0;n--)
		{
				puts(message);
				sleep(1);
		}

		//부모 프로세스인 경우 종료처리
		if(pid!=0)
		{
				int stat_val;
				pid_t child_pid;

				child_pid=wait(&stat_val);
				printf("Child has finished: PID = %d\n",child_pid);
				pr_exit(status);
		}
		exit(exit_code);
}

