//ex copy a.txt b.txt

#include <stdio.h>  //printf
#include <fcntl.h>	//O_RDONLY, O_WRONLY, ..
#include <unistd.h> //write(), read(), close()
#define BUF_SIZE 1
#define DEBUG


int main(int argc, char *argv[])
{
#ifdef DEBUG
	printf("argc=%d\n",argc);
	printf("argv[1] =%s\n",argv[1]);
	printf("argv[2] =%s\n",argv[2]);
#endif

	int fd1, fd2, len;
	char buf[BUF_SIZE];

	//open read file
	fd1=open(argv[1],O_RDONLY); //oepn read file
	if(fd1==-1)
			printf("%s open error\n",argv[1]);

	fd2=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC); //open write file
		if(fd2==-1)	
			printf("%s open error\n",argv[2]);

	while(len=read(fd1,buf,sizeof(buf))>0)//filename address bufsize
	{									  //len > 0 =>  can read
		write(fd2,buf,len);
	}
	close(fd1);
	close(fd2);
	return 0;
}
