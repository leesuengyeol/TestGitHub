#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

//File TO Key
//key_t ftok(const char *pathname, int proj_id,pathname,id)

int main(int argc, char **argv)
{
	key_t msgKey;

	msgKey = ftok("/home/pi/rasspberrypiEx/linux",'A');//아무 프로세서든 받지못하도록 key값을 설정
	printf("ftok_key=%d\n", msgKey);

	return 0;
}

