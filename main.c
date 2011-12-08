#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include "keyboard.h"
#include "main.h"
#include "framebuffer.h"
#include "draw.h"
#include "image.h"
#include "directory.h"
#include "font.h"
#include "text.h"
#include "weather.h"

uint16_t global_keycode;
Com_box *img_list;
Mp3_box *mp3_list;
Tframebuffer *fb;
FB_FONT *font;
unsigned char *weather_info;

void init();

int main(int argc , char **argv)
{

	printf("%s\n" , FB_PATH);
	printf("%s\n" , JPG_PATH);
	printf("%s\n" , MP3_PATH);

	init();
//	fprintf(stdout ,"img_list length=%d\n" ,img_list->length);

	pthread_t key_controll_pthread = start_key_controll_pthread();



	int flag = 1;
//	uint16_t current_keycode = 255;

	while(flag)
	{
//		printf("in while\n");	
		if(global_keycode == KEY_Q)
		{
			flag = 0;
		}/*
		else if(global_keycode == current_keycode)
		{
			sleep(1);
			continue;
		}
		
		current_keycode = global_keycode;	
		*/
		else
		{	
			
		}
		sleep(1);	
	}

	//int pthread_join(pthread_t thread, void **value_ptr);	
	if((pthread_join( key_controll_pthread , NULL)) != 0)
	{
				
		fprintf( stdout , "[%d] pthread join error\n" ,(int) key_controll_pthread);
		exit(-1);
	}

	return 0;

}

void init()
{
	fb = malloc(sizeof(Tframebuffer));
	fb = init_frame_buffer(fb , FB_PATH );
	img_list = malloc(sizeof(Com_box));
	img_list = img_search( JPG_PATH , img_list);
	mp3_list = malloc(sizeof(Mp3_box));
	mp3_list = mp3_search( MP3_PATH , mp3_list);
	init_font();
	//int weather_getinfo(unsigned char *weatherinfo, int second)
	weather_getinfo(weather_info , 10);
//	printf("%s" , weather_info);

}
