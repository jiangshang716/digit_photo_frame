#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <jpeglib.h>
#include <jerror.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <syslog.h>
#include "directory.h"
Mp3_box  *mp3_search(char *dirname , Mp3_box * list)
{
	
	char *fname = NULL;
	int num = 0;
	char *dirname2 = (char *)malloc(sizeof(char) * 128);

	
	
	DIR *dp;
	struct dirent *entry;	
	struct stat statbuf;

	
//	fprintf(stdout , "begin open dir=%s\n" , dirname);	
	if((dp=opendir(dirname))==NULL)	//打开的目录为当前目录路下的picture目录
	{
		fprintf( stderr , "err: failed to opendir %s : %s\n" , dirname , strerror(errno));
		exit(1);
	}
	
//	fprintf(stdout , "open dir=%s  success\n" , dirname);	

	chdir(dirname);		//切换到当前目录	
	while((entry=readdir(dp)) != NULL)//读取指定文件夹下图像文件名（字符串）放到内存 用指针fname指向这个字符串
	{
		lstat(entry->d_name, &statbuf);		//获取读到的文件属性
		
		if(S_ISDIR(statbuf.st_mode))
		{
		
			if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0)
			{
				continue;
			}
			sprintf(dirname2, "%s/%s", dirname, entry->d_name);
			mp3_search(dirname2 , list);
			
		
		}
		else
		{
			

			int len = strlen(entry->d_name);
			len -= 4;
			
			if (strcmp(entry->d_name + len, ".MP3")!=0 && strcmp(entry->d_name + len, ".mp3")!=0)
			{
				continue;
			}
			else
			{
				fname = entry->d_name;
//				fprintf(stdout , "pName=%s\n",entry->d_name);
//				fprintf(stdout , "dirName=%s\n",dirname);
//				fprintf(stdout , "fName=%s\n",fname);
				memset(list->mp3_list[num], 0, 255);	
				snprintf(list->mp3_list[num],255, "%s/%s", dirname, fname);
//				printf("abs name=%s\n", list->img_list[num]);
				
				num++;		
			}
		}

	}
	
	chdir("..");
	closedir(dp);
	
	list->length = num;
	list->current_idx = 0;	

	return list;
	
}



Com_box  *img_search(char *dirname , Com_box * list)
{
	
	char *fname = NULL;
	int num = 0;
	char *dirname2 = (char *)malloc(sizeof(char) * 128);

	
	
	DIR *dp;
	struct dirent *entry;	
	struct stat statbuf;

	
//	fprintf(stdout , "begin open dir=%s\n" , dirname);	
	if((dp=opendir(dirname))==NULL)	//打开的目录为当前目录路下的picture目录
	{
		fprintf(stderr , "err: failed to opendir %s :%s\n" , dirname ,  strerror(errno));
		exit(1);
	}
	
//	fprintf(stdout , "open dir=%s  success\n" , dirname);	

	chdir(dirname);		//切换到当前目录	
	while((entry=readdir(dp)) != NULL)//读取指定文件夹下图像文件名（字符串）放到内存 用指针fname指向这个字符串
	{
		lstat(entry->d_name, &statbuf);		//获取读到的文件属性
		
		if(S_ISDIR(statbuf.st_mode))
		{
		
			if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0)
			{
				continue;
			}
			sprintf(dirname2, "%s/%s", dirname, entry->d_name);
			img_search(dirname2 , list);
			
		
		}
		else
		{
			

			int len = strlen(entry->d_name);
			len -= 4;
			
			if (strcmp(entry->d_name + len, ".jpg")!=0 && strcmp(entry->d_name + len, ".JPG")!=0)
			{
				continue;
			}
			else
			{
				fname = entry->d_name;
//				fprintf(stdout , "pName=%s\n",entry->d_name);
//				fprintf(stdout , "dirName=%s\n",dirname);
//				fprintf(stdout , "fName=%s\n",fname);
				memset(list->img_list[num], 0, 255);	
				snprintf(list->img_list[num],255, "%s/%s", dirname, fname);
//				printf("abs name=%s\n", list->img_list[num]);
				
				num++;		
			}
		}

	}
	
	chdir("..");
	closedir(dp);
	
	list->length = num;
	list->current_idx = 0;	

	return list;
	
}



#if 0

int resource_common_list(char *usbpath, char list[][DB_NAME_MAX + 1], char *type)
{
	DIR *dirp;
	char path_com[DB_NAME_MAX + 1];
	char *iroot;
	int list_len;

	root = "digitbox";

	memset(path_com, 0, sizeof(path_com));


	/* make path like <usbpath>/mp3(or other) */
	//int snprintf(char *str, size_t size, const char *format, ...);
	snprintf(path_com, sizeof(path_com) - 1, "%s/%s/%s", usbpath, root, type);

	fprintf(stdout , "usbpath=%s\n" , path_com);	

	//DIR *opendir(const char *name);
	if((dirp = opendir(path_com)) == NULL){
		fprintf(stderr, "%s:%d: opendir %s failed: %s\n", __FILE__, 
			__LINE__, path_com, strerror(errno));
		
		return -1;
	}

	//int resource_get_list(DIR *dirp, char list[][DB_NAME_MAX + 1], char *type);
	list_len = resource_get_list(dirp, list, path_com, type);

	//int closedir(DIR *dirp);
	closedir(dirp);

	return list_len;		/* list length */
}


/**
	接收一个文件夹做参数，在此文件夹里搜索.mp3文件，
	文件名全部放到数组里，然后循环播放这些音频文件
**/

void func_mus(char *dirname , char * type1 , char * type2)
{
	int j = 0;

	char *fname = NULL;

	DIR *dp;
	struct dirent *entry;
	
	if((dp=opendir(dirname))==NULL)	//打开的目录为当前目录路下的picture目录
	{
		perror("err: fail to opendir\n");
		return;
	}

	char *fname2 = (char *)malloc(sizeof(char) * 64);	//如果分配太小，很可能放不下文件名（含路径）
	char *fname_array[BUFSZ];
	int i = 0;
		
	while((entry=readdir(dp)) != NULL)//读取指定文件夹下图像文件名（字符串）放到内存 用指针fname指向这个字符串
	{
		if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0)
			continue;
		
		//判断是否为音频文件
		int len = strlen(entry->d_name);
		len -= 4;
		if (strcmp(entry->d_name + len, type1)!=0 && strcmp(entry->d_name + len, type2)!=0)
			continue;

		else
		{
			fname = entry->d_name;
			//printf("%s\n", fname);

			sprintf(fname2, "%s/%s", dirname, fname);	//连接字符串，构造一个完整的文件名
			fname_array[i] =  (char *)malloc(sizeof(char) * 64);
			strcpy(fname_array[i], fname2);	
			//printf("%s\n", fname_array[i]);

			i++;
		}

	}

	
	
	free(fname2);
	printf("%d\n", i);
/*
	for(j = 0; j < i;)
	{
		printf("%s\n", fname_array[j]);
		func_mus2(fname_array[j]);		//执行一次，调用madplay之后可能出错而导致程序退出，无法执行下面的语句
		j++;
		//for循环一次之后，再次重新循环！！！！！！！！！！！！
		if (j == i)	
		{
			j = 0;
		}
*/
	}

}
#endif 
