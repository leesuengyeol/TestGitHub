#include <unistd.h> //open()
#include <fcntl.h>  //O_CREAT
#include <stdio.h> 

int main(void)
{
	int fd;
	int val;
	fd = open("exec_copy.txt.", O_CREAT|O_TRUNC);
	
	//loop program make 'execl'
	execl("./loop","./loop",(char*)0);
	return 0;
}

