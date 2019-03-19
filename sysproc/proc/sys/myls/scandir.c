#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
struct dirent
{
long d_ino; //inode 번호
off_t d_off; // offset
unsigned short d_reclen; //d_name 길이
char d_name[NAME_MAX+1]; //파일 이름
};
*/

const char *path =".";

int main(int argc ,char *argv[])
{
		struct dirent **namelist;
		int count;
		int idx;
		if(argc<2)
		{
			printf("argv[1] is empty\n");
			return -1;
		}


		if((count =scandir(argv[1],&namelist, NULL,alphasort)) ==-1) //scandir 에러시 -1 반환, 성공시 namelist 																										갯수 반환
		{
			fprintf(stderr, "%s 디렉토리 조회 오류: %s\n",argv[1],strerror(errno));
			return 1;
		}

		for(idx =0 ; idx<count; idx++) //count =namelist배열의 원소 갯수
		{
				printf("%s\n" ,namelist[idx]->d_name);
		}
		//건별 데이터 메모리 해제
		for(idx=0;idx<count; idx++){
				free(namelist[idx]);
		}

		//namelist 에 대한 메모리 해제
		free(namelist);
		return 0;
}
