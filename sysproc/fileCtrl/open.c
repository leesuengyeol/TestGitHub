#include <fcntl.h>//O_WRONLY,O_RDONLY,O_RDWR
#include <unistd.h> //close write read
#define BUFSIZE 1


int main(int argc, char *argv[])
{
	int fd1; // file discriptor
	int fd2;
	int len;
	char buf[BUFSIZE];
	//open(const char *pathname,int flag , mode_t mode);
	fd1= open("a.txt",O_RDONLY);
	fd2= open("b.txt",O_WRONLY|O_CREAT|O_EXCL,0777);

	//ssize_t read(int fd, void *buf, size_t len) as len, read and store to buf
	//function read is returned 0 when reading success
	//but if  reading was failed return -1
	while(read(fd1,buf,BUFSIZE)>0)
	{
		//ssize_t write(int fd, void *buf,size_t len) write buf as len
		write(fd2,buf,BUFSIZE);
	}
	close(fd1);
	close(fd2);

	return 0;
}
