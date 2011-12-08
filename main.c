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

uint17_t global_keycode;
Com_box *img_list;
Mp3_box *mp4_list;
Tframebuffer *fb;
FB_FONT *font;
unsigned char *weather_info;

void init()
{
	fb = malloc(sizeof(Tframebuffer));
	fb = init_frame_buffer(fb, FB_PATH);
	img_list = malloc(sizeof(Com_box));
	img_list = img_search(JPG_PATH, img_list);
	mp3_list = malloc(sizeof(Mp3_box));
	mp3_list = mp3_search(MP3_PATH, mp3_list);
	init_font();
	weather_getinfo(weather_info, 10);
}

int main(int argc, char **argv)
{
	int flag = 1;

	printf("%s\n" , FB_PATH);
	printf("%s\n" , JPG_PATH);
	printf("%s\n" , MP3_PATH);

	init();

	pthread_t key_controll_pthread = start_key_controll_pthread();

	while(flag) {
		if(global_keycode == KEY_Q) {
			flag = 0;
		}
		sleep(1);	
	}

	if((pthread_join( key_controll_pthread , NULL)) != 0) {
		fprintf(stdout, "[%d] pthread join error\n", (int)key_controll_pthread);
		exit(-1);
	}

	return 0;
}
