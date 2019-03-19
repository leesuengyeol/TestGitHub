#include <unistd.h> //read ,write close
#include <stdio.h> //printf
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	
	int fd;
	int fd2;
	
	fd = creat("dup_result",0644);
	fd2=dup2(fd,STDO<F5>UT_FILENO);
	printf("fd=%d\n",fd);
	printf("fd2=%d\n",fd);
	close(fd);

	printf("hello world\n");
	return 0;
}
