#include <stdio.h>
#include <unistd.h>
#include <dirent.h> //using struct DIR 
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	DIR *dir_info;
	struct dirent *dir_entry;

	mkdir("makeA",0755);
	mkdir("makeB",0755);

	dir_info=opendir(".");

	if(NULL!=dir_info)
	{
		while(dir_entry=readdir(dir_info));
		{
			printf("%s\n",dir_entry->d_name);
		}
		closedir(dir_info);
	}
	return 0;
}
