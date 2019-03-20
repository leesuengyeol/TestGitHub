#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
		int fd;
		fd= open("/home/lee/temp/1.txt",O_RDONLY);
		return 0;
}
