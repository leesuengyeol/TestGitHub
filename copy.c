//ex copy a.txt b.txt

#include <stdio.h>  //printf
#include <fcntl.h>	//O_RDONLY, O_WRONLY, ..
#include <unistd.h> //write(), read(), close()
#define BUF_SIZE 1

int main(int arg, char*argv[])
{
	int fd1, fd2, len;
	char buf[BUF_SIZE];

	//open read file
	fd1=open("a.txt",O_RDONLY); //oepn read file
	fd2=open("b.txt",O_WRONLY|O_CREAT|O_TRUNC,0644); //open write file

	while(len=read(fd1,buf,sizeof(buf))>0)//filename address bufsize
	{									  //len > 0 =>  can read
		write(fd2,buf,len);
	}
	close(fd1);
	close(fd2);
	return 0;
}
