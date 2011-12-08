#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdint.h>
#include "main.h"
#include "keyboard.h"
#include "directory.h"
#include "framebuffer.h"
#include "draw.h"
#include "image.h"
#include "music.h"
#include "weather.h"

pthread_mutex_t keycode_mutex = PTHREAD_MUTEX_INITIALIZER;

extern uint16_t global_keycode;

extern Com_box *img_list;
extern Tframebuffer *fb;
extern Mp3_box *mp3_list;
extern Tframebuffer *fb;
extern unsigned char *weather_info;

pthread_t music_pthread;
pthread_t auto_display_pthread;
extern pthread_mutex_t music_mutex;

void first_show();
void show_next();
void show_privious();
void privious_music();
void next_music();

pthread_t start_key_controll_pthread()
{

	pthread_t key_pthread_id;
	//int pthread_create(pthread_t *restrict thread,const pthread_attr_t *restrict attr,void *(*start_routine)(void*), void *restrict arg);
	
	int ret_int = pthread_create( &key_pthread_id , NULL , key_controll_pthread_fun , NULL);
	
	if(ret_int != 0)
	{
		fprintf(stderr , "%s error\n" , __func__);
		exit(-1);
	}

	music_pthread = start_music_pthread();
	
	return key_pthread_id;
}

void *key_controll_pthread_fun()
{
	int keyboard_fd;
	char *keyboard_str = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";	

	//int open(const char *pathname, int flags);
	//int open(const char *pathname, int flags, mode_t mode);
	//int creat(const char *pathname, mode_t mode);
	if((keyboard_fd = open(keyboard_str , O_RDONLY )) < 0)
	{
		fprintf( stderr , "open %s failed:%s\n" , keyboard_str , strerror(errno));
		exit(1);
	}
//	fprintf( stdout , "open %s success\n" , keyboard_str );
#if 0
	struct input_event {
        struct timeval time;
        __u16 type;
        __u16 code;
        __s32 value;
    };	
#endif
	struct input_event i_event;
	int read_bytes;

	int auto_display = 0;
	int keep_reading = 1;
	
	clear_frame_buffer( fb );
	first_show();
	show_weather(fb , weather_info);	

	while(keep_reading)
	{
//		fprintf(stdout , "1\n");
		

		if((read_bytes = read(keyboard_fd , &i_event , sizeof(struct input_event)))	< 0)
		{
			fprintf(stdout , "read from keyboard error:%s\n" , strerror(errno));
			exit(1);
		}
		else 
		{
			// press key
			if(EV_KEY == i_event.type)
			{
				// value == 1 means key DOWN  however 2 means key UP
				if(i_event.value == 1)
				{
				
					//fprintf(stdout , "key DOWN value=%d\n" , i_event.value);		

					switch(i_event.code)
					{
						case KEY_LEFT :
									
							change_global_keycode(&global_keycode , i_event.code);
					//		clear_frame_buffer( fb );
					//			pthread_cancel(auto_display_pthread);
							show_privious();
					//		show_weather(fb , weather_info);	
							break;
						
						case KEY_RIGHT:
							change_global_keycode(&global_keycode , i_event.code);
					//		clear_frame_buffer( fb );
					//		pthread_cancel(auto_display_pthread);
							show_next();						
					//		show_weather(fb , weather_info);	
							break;
						case KEY_N:
							change_global_keycode(&global_keycode , i_event.code);
							pthread_cancel(music_pthread);
							system("killall -9 madplay");
					//		pthread_mutex_lock(&music_mutex);
							next_music();
							if(auto_display == 0)
							{
								
					//			pthread_mutex_lock(&music_mutex);
								clear_frame_buffer( fb );
								first_show();
								show_weather(fb , weather_info);	
					//			pthread_mutex_lock(&music_mutex);
							}

							break;
						case KEY_P:
							change_global_keycode(&global_keycode , i_event.code);
							pthread_cancel(music_pthread);
							system("killall -9 madplay");
						    
							//int pthread_mutex_lock(pthread_mutex_t *mutex);	
					//		pthread_mutex_lock(&music_mutex);
							privious_music();
							if(auto_display == 0)
							{
								
					//			pthread_mutex_lock(&music_mutex);
								clear_frame_buffer( fb );
								first_show();
								show_weather(fb , weather_info);	
					//			pthread_mutex_lock(&music_mutex);
							}
							
							break;
						case KEY_SPACE:
							if(auto_display == 0)
							{

								auto_display = 1;
								auto_display_pthread = start_auto_display_pthread();	
							}		
							else
							{
								
								auto_display = 0;
								pthread_cancel(auto_display_pthread);
								clear_frame_buffer( fb );
								first_show();		
								show_weather(fb , weather_info);	
							}
							break;
						case KEY_Q:
							change_global_keycode(&global_keycode , i_event.code);
							pthread_cancel(auto_display_pthread);
							pthread_cancel(music_pthread);
							system("killall -9 madplay");
							goto END;
					}
				}
#if 0
				else if(i_event.value == 0)
				{
					
					fprintf(stdout , "key UP value=%d\n" , i_event.value);
				}
				else
				{
					fprintf(stdout , "do not know value=%d\n" , i_event.value);
				}
#endif

			}
		}	

	}
END:
	fprintf(stdout ,"key_controll_pthread exit : %s\n" , __func__);
	//int close(int fd);
	close(keyboard_fd);

	//void pthread_exit(void *value_ptr);
	pthread_exit(NULL);	

}

int change_global_keycode( uint16_t *dest , uint16_t source)
{
	//int pthread_mutex_lock(pthread_mutex_t *mutex);
	// if  successful,  the  pthread_mutex_lock() and pthread_mutex_unlock() functions shall return zero;
	// otherwise, an error number shall be returned to indicate the error.
	if (pthread_mutex_lock(&keycode_mutex) != 0)
	{
		fprintf(stdout , "mutex keycode error");
		exit(1);
	}
	
	*dest = source;

    //int pthread_mutex_unlock(pthread_mutex_t *mutex);
	if (pthread_mutex_unlock(&keycode_mutex) != 0)
	{
		fprintf(stdout , "mutex keycode error");
		exit(1);
	}

	return 0;
}

void show_next()
{

	Tjpeg *jp = malloc(sizeof(Tjpeg));
	jp->file_name = img_list->img_list[img_list->current_idx];
	
	if(img_list->current_idx == ( img_list->length - 1))
	{
		img_list->current_idx = 0;
	}
	else
	{
		
		img_list->current_idx ++;
	}

	jp = get_image_buffer(jp);
	draw_to_screen_begin_line(fb , jp , 50);
	free(jp);


}

void first_show()
{


	Tjpeg *jp1 = malloc(sizeof(Tjpeg));
	jp1->file_name = img_list->img_list[img_list->current_idx];
	jp1 = get_image_buffer(jp1);
	draw_to_screen_begin_line(fb , jp1 ,50);
	free(jp1);
}

void show_privious()
{


	Tjpeg *jp1 = malloc(sizeof(Tjpeg));
	jp1->file_name = img_list->img_list[img_list->current_idx];
	
	if(img_list->current_idx == 0)
	{
		img_list->current_idx = img_list->length - 1;
	}
	else
	{
		
		img_list->current_idx --;
	}

	jp1 = get_image_buffer(jp1);
	draw_to_screen_begin_line(fb , jp1 ,50);
	free(jp1);
	


}

void next_music()
{
	
	
	if((mp3_list->current_idx+1) == mp3_list->length)
	{
		mp3_list->current_idx = 0;
	}
	else
	{
		mp3_list->current_idx ++;
	}

	music_pthread = start_music_pthread();

}

void privious_music()
{


	if((mp3_list->current_idx - 1) < 0)
	{
		mp3_list->current_idx = mp3_list->length - 1;
	}
	else
	{
		mp3_list->current_idx --;
	}

	music_pthread = start_music_pthread();


}

