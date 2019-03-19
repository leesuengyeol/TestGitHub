#include <grp.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h> // getpwuid
#include <time.h>

void access_perm(char *perm, mode_t mode)
{
		int i;
		char permchar[] = "rwx";
		memset(perm,'-',10);
		perm[10]='\0';
		if(S_ISDIR(mode)) perm[0] = 'd';
		else if (S_ISCHR(mode)) perm[0] = 'c';
		else if (S_ISBLK(mode)) perm[0] ='b';
		else if (S_ISFIFO(mode)) perm[0] = 'p';
		else if (S_ISLNK(mode)) perm[0] = 't';

		for(i = 0; i<9;i++)
		{
			if((mode << i) & 0x100)
					perm[i+1] = permchar[i%3];
		}
		if(mode & S_ISUID) perm[3] = 's';
		if(mode & S_ISGID) perm[6] ='s';
		if(mode & S_ISVTX) perm[9] ='t';
}

int main(int argc, char *argv[])
{
		DIR *dp;
		struct stat statbuf;
		struct dirent *dent;
		char perm[11];
		char pathname[80];
		struct passwd *username;
		struct group *groupname;
		struct tm *tm;
		char buf[30];
		if(argc<2) exit(1);

		
		stat(argv[1], & statbuf);
		if(!S_ISDIR(statbuf.st_mode))
		{
				fprintf(stderr," %s is not directory\n",argv[1]);
				exit(1);
		}
		if((dp = opendir(argv[1])) ==NULL)
		{
				perror("Error:");
				exit(1);
		}
		printf("List of Directory(%s):\n",argv[1]);

		while((dent =readdir(dp))!=NULL)
		{
				sprintf(pathname,"%s/%s", argv[1], dent->d_name);
				lstat(pathname, &statbuf);
				access_perm(perm,statbuf.st_mode);
				username=getpwuid(statbuf.st_uid);
				groupname =getgrgid(statbuf.st_gid);
				tm = localtime(&statbuf.st_ctime);
				strftime(buf,sizeof(buf),"%m월 %e %H:%M",tm);
				if (!strcmp(argv[2],"-al"))
				{
				printf("%s %s %s\t%8ld\t%s\t%s \n", perm,username->pw_name ,groupname->gr_name ,statbuf.st_size,buf,dent->d_name);
				}
				else if(!strcmp(argv[2],"-a"))
				{
					printf("%s\t",dent->d_name);
				}
				else if(argv[2]==NULL)
				{
					if(S_ISDIR(statbuf.st_mode||S_ISREG(statbuf.st_mode)))
									{
									printf("%s\t",dent->d_name);
									}
				}
		}
		if((!strcmp(argv[2],"-a"))|argv[2]==NULL) 
		{
		printf("\n");
		}
		closedir(dp);
		return 0;
}
