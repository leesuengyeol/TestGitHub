#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "123";
	char buffer[BUFSIZ +1]; //proc의 버퍼 지정
	
	memset(buffer,'\0',sizeof(buffer));

	if(pipe(file_pipes) ==0)
	{
		data_processed = write (file_pipes[1],some_data,strlen(some_data)); //file_pies[1]에 write some_data를 strlen(some_data)만큼
		printf("Wrote %d bytes\n",data_processed); //data_processed는 write해온 만큼의 byte
		data_processed =read(file_pipes[0],buffer,BUFSIZ);//file_pipes[0]에 버퍼에 있는 걸 BUFSIZ만큼 쓰겠다
		printf("Read %d bytes: %s\n",data_processed,buffer);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}
