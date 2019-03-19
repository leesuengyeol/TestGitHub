#include <stdio.h>
#include <fcntl.h> //flag arg ex)O_RDONLY, O_WRONLY, O_RDWR
#include <unistd.h> //write ,close ,read ...
#define BUF_SIZE 1
#define DEBUG

int main(int argc, char *argv[])
{
#ifdef DEBUG
	printf("argc=%d\n",argc);
	printf("argv[1]=%s\n",argv[1]);
	printf("arvg[2]=%s\n",argv[2]);
#endif
	int fd1,fd2;
	char buf[BUF_SIZE];

	fd1=open(argv[1],O_RDONLY);
	fd2=open(argv[2],O_WRONLY|O_CREAT|O_EXCL,0777);

	while(read(fd1,buf,BUF_SIZE)>0)
	{
		write(fd2,buf,BUF_SIZE);
	}

	close(fd1);
	close(fd2);

	return 0;
}


