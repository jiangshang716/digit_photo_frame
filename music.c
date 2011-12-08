#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "music.h"
#include "directory.h"
#include "main.h"

extern Mp3_box *mp3_list;

pthread_mutex_t music_mutex = PTHREAD_MUTEX_INITIALIZER;


pthread_t start_music_pthread()
{

    //int pthread_create(pthread_t *restrict thread,const pthread_attr_t *restrict attr,void *(*start_routine)(void*), void *restrict arg);
	pthread_t pth_id;
	
	int ret_int = pthread_create( &pth_id , NULL , play_music_pthread_fun , NULL);

    if(ret_int != 0)
    {
        fprintf(stderr , "%s error\n" , __func__);
        exit(-1);
    }
    return pth_id;
	
}

void *play_music_pthread_fun()
{
	
	char cmd[255];
	int i;
	printf("start music\n");
//	printf("%d\n" , mp3_list->current_idx);
//	printf("%s\n" , mp3_list->mp3_list[mp3_list->current_idx]);
	for( i = mp3_list->current_idx ; i < mp3_list->length ; i ++ )
	{
		sprintf(cmd , "madplay %s ", mp3_list->mp3_list[i]);
		system( cmd );
		//int pthread_mutex_unlock(pthread_mutex_t *mutex);
		
		//pthread_mutex_unlock(&music_mutex);

		if( i == mp3_list->length - 1)
		{
			i = 0;
		}
	
	}
		
	printf("end music\n");
	pthread_exit(NULL);	
	
}
