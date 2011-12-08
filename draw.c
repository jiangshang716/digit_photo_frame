#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "draw.h"
#include "main.h"
#include "directory.h"

extern Tframebuffer *fb;
extern Com_box *img_list;

pthread_t start_auto_display_pthread()
{
	pthread_t auto_display_pthread_id;

	int ret_int = pthread_create( &auto_display_pthread_id , NULL , 
									auto_display_pthread_fun , NULL);

	    if(ret_int != 0)
		{
			fprintf(stderr , "%s error\n" , __func__);
			exit(-1);
		}

	return auto_display_pthread_id;
}

void *auto_display_pthread_fun()
{
	srand( (unsigned)time( NULL ) ); 
	//	   printf("Random number in the 0-99 range: %d\n", random (100)); 
	int c_case;
	while(1)
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
		//c_case = rand() % 4;	
		c_case = 4;	
		switch(c_case)
		{
			case 0:
					
				//void draw_to_screen_up_to_down( Tframebuffer *fb , Tjpeg *jp ,int begin)
				draw_to_screen_up_to_down( fb , jp , 0);
				sleep(1);
				break;
			case 1:
				draw_to_screen_down_to_up( fb , jp , 0);
				sleep(1);
				break;

			case 2:

				draw_to_screen_left_to_right( fb , jp , 0);
				sleep(1);
				break;

			case 3:
				draw_to_screen_right_to_left( fb , jp , 0);
				sleep(1);
	
				break;

			case 4:

				draw_to_screen_up_to_down_plus( fb , jp , 0);
				sleep(1);
				break;
			case 5:

				break;
			case 6:

				break;
			case 7:

				break;
			case 8:

				break;


		}
		free(jp);	
	}

		
	pthread_exit(NULL);	

}

void draw_to_screen_begin_line( Tframebuffer *fb , Tjpeg *jp ,int begin)
{
	
	unsigned long screen_size = fb->screensize;
	short screen_bits_per_pixel = fb->fb_vinfo.bits_per_pixel;
	unsigned char * fb_start = fb->fb_start;
	unsigned char * image_buffer = jp->image_buffer;
	short image_bytes_per_pixel = jp->jpeg.num_components;
	unsigned long image_size = jp->image_size;
	unsigned long begin_location = begin * fb->fb_vinfo.xres * 4;

	if(screen_bits_per_pixel == 32){
		unsigned char *temp = malloc( screen_size  );
		//void *memset(void *s, int c, size_t n);
		memset(temp , 0 , screen_size / 4);

		if (temp == NULL)
		{
			fprintf(stderr , "malloc error\n" );
			exit(-1);

		}

		int a;
		int b;

		for( a = 0,b = 0 ; a < image_size  ; 
				a += image_bytes_per_pixel , b += screen_bits_per_pixel / 8){
			unsigned int color = c24_to_32(image_buffer[a] , 
											image_buffer[a+1] , 
											image_buffer[a+2]);
			
			temp[b] = getuc(color , 4);
			temp[b+1] = getuc(color , 3);
			temp[b+2] = getuc(color , 2);
			temp[b+3] = getuc(color , 1);
			
		}

		unsigned long  len = fb->screensize ;
		fb_start += begin_location;
		memcpy( fb_start  , temp  , len - begin_location);
		
		free(temp);
	}
}

void draw_to_screen_left_to_right( Tframebuffer *fb , Tjpeg *jp ,int begin)
{
	
	short screen_bits_per_pixel = fb->fb_vinfo.bits_per_pixel;
	unsigned char * fb_start = fb->fb_start;
	unsigned long begin_location = begin * fb->fb_vinfo.xres * 4;

	if(screen_bits_per_pixel == 32){
			
		unsigned char *temp = get_format_to_32_jpg(fb , jp);
		int ti;
		int tx;
		unsigned long  len = jp->jpeg.image_width*4;
		fb_start += begin_location;
		for (tx = 0 ; tx < jp->jpeg.image_width * 4 ; tx ++)
		{
	
			for(ti = jp->jpeg.image_height ; ti >= 0 ; ti --)
			{
				if( (ti * len + begin_location ) >= fb->screensize)
				{
					//fprintf(stdout , "out of bound break\n");
					continue;
				}
				memcpy( fb_start + ti * len + tx, temp + ti *len + tx , 4);

			}
			usleep(50);
		}
		
		free(temp);
	}
}


void draw_to_screen_right_to_left( Tframebuffer *fb , Tjpeg *jp ,int begin)
{
	
	short screen_bits_per_pixel = fb->fb_vinfo.bits_per_pixel;
	unsigned char * fb_start = fb->fb_start;
	unsigned long begin_location = begin * fb->fb_vinfo.xres * 4;

	if(screen_bits_per_pixel == 32){
			
		unsigned char *temp = get_format_to_32_jpg(fb , jp);
		int ti;
		int tx;
		unsigned long  len = jp->jpeg.image_width*4;
		fb_start += begin_location;
		for (tx = jp->jpeg.image_width * 4 ; tx > 0 ; tx --)
		{
	
			for(ti = jp->jpeg.image_height ; ti >= 0 ; ti --)
			{
				if( (ti * len + begin_location ) >= fb->screensize)
				{
					//fprintf(stdout , "out of bound break\n");
					continue;
				}
				memcpy( fb_start + ti * len + tx, temp + ti *len + tx , 4);

			}
			usleep(50);
		}
		
		free(temp);
	}
}

void draw_to_screen_down_to_up( Tframebuffer *fb , Tjpeg *jp ,int begin)
{
	
	short screen_bits_per_pixel = fb->fb_vinfo.bits_per_pixel;
	unsigned char * fb_start = fb->fb_start;
	unsigned long begin_location = begin * fb->fb_vinfo.xres * 4;

	if(screen_bits_per_pixel == 32){
			
		unsigned char *temp = get_format_to_32_jpg(fb , jp);
		int ti;
		unsigned long  len = jp->jpeg.image_width*4;
		fb_start += begin_location;
		
		for(ti = jp->jpeg.image_height ; ti >= 0 ; ti --)
		{
			if( (ti * len + begin_location ) >= fb->screensize)
			{
				//fprintf(stdout , "out of bound break\n");
				continue;
			}
			memcpy( fb_start + ti * len, temp + ti *len , len);
			usleep(1000);
		
		}
		
		free(temp);
	}
}



void draw_to_screen_up_to_down_plus( Tframebuffer *fb , Tjpeg *jp ,int begin)
{
	
	short screen_bits_per_pixel = fb->fb_vinfo.bits_per_pixel;
	unsigned char * fb_start = fb->fb_start;
	unsigned long begin_location = begin * fb->fb_vinfo.xres * 4;

	if(screen_bits_per_pixel == 32)
	{
			
		unsigned char *temp = get_format_to_32_jpg(fb , jp);
		int t1 = 0,t2 = 101,t3 = 201, t4 = 301 ,t5= 401 ,t6 = 501 ;
		unsigned long  len = jp->jpeg.image_width*4;
		fb_start += begin_location;
	
//	int which = 1;
	int tt;

	for( tt = 0 ; tt < jp->jpeg.image_height ; tt ++)		
	{

		for(  ; t1 <= 100 ; )
		{
			memcpy( fb_start + t1 * len, temp + t1 *len , len);
			t1++ ;
			usleep(1000);
			break;
		}
		for( ; t2 <= 200 ; )
		{
			memcpy( fb_start + t2 * len, temp + t2 *len , len);
			t2 ++;
			usleep(1000);
			break;
		}
		for(; t3 <= 300 ; )
		{
			memcpy( fb_start + t3 * len, temp + t3 *len , len);
			t3 ++;
			usleep(1000);
			break;
		}
		for( ; t4 <= 400 ; )
		{
			memcpy( fb_start + t4 * len, temp + t4 *len , len);
			t4 ++;
			usleep(1000);
			break;
		}
		for(; t5 <= 500 ; )
		{
			memcpy( fb_start + t5 * len, temp + t5 *len , len);
			t5 ++;
			usleep(1000);
			break;
		}
		for(; t6 <= 600 ;)
		{
			memcpy( fb_start + t6 * len, temp + t6 *len , len);
			t6 ++;
			usleep(1000);
			break;
		}



	}

	free(temp);
	}
}
void draw_to_screen_up_to_down( Tframebuffer *fb , Tjpeg *jp ,int begin)
{
	
	short screen_bits_per_pixel = fb->fb_vinfo.bits_per_pixel;
	unsigned char * fb_start = fb->fb_start;
	unsigned long begin_location = begin * fb->fb_vinfo.xres * 4;

	if(screen_bits_per_pixel == 32){
			
		unsigned char *temp = get_format_to_32_jpg(fb , jp);
		int ti;
		unsigned long  len = jp->jpeg.image_width*4;
		fb_start += begin_location;
		
		for(ti = 0 ; ti < jp->jpeg.image_height ; ti ++)
		{
			if( (ti * len + begin_location ) >= fb->screensize)
			{
				//fprintf(stdout , "out of bound break\n");
				break;
			}
			memcpy( fb_start + ti * len, temp + ti *len , len);
			usleep(1000);
		
		}
		
		free(temp);
	}
}


unsigned char *get_format_to_32_jpg(  Tframebuffer *fb , Tjpeg *jp  )
{
	unsigned long screen_size = fb->screensize;
	short screen_bits_per_pixel = fb->fb_vinfo.bits_per_pixel;
	unsigned char * image_buffer = jp->image_buffer;
	short image_bytes_per_pixel = jp->jpeg.num_components;
	unsigned long image_size = jp->image_size;
	unsigned char *temp = malloc( screen_size  );
	//void *memset(void *s, int c, size_t n);
	memset(temp , 0 , screen_size / 4);

	if (temp == NULL)
	{
		fprintf(stderr , "malloc error\n" );
		exit(-1);

	}
	
	int a;
	int b;
	
	for( a = 0,b = 0 ; a < image_size  ; 
			a += image_bytes_per_pixel , b += screen_bits_per_pixel / 8){
		unsigned int color = c24_to_32(image_buffer[a] , 
										image_buffer[a+1] , 
										image_buffer[a+2]);
		
		temp[b] = getuc(color , 4);
		temp[b+1] = getuc(color , 3);
		temp[b+2] = getuc(color , 2);
		temp[b+3] = getuc(color , 1);
		
	}
	
	return temp;


}


void draw_to_screen( Tframebuffer *fb , Tjpeg *jp )
{
	
	unsigned long screen_size = fb->screensize;
	short screen_bits_per_pixel = fb->fb_vinfo.bits_per_pixel;
	unsigned char * fb_start = fb->fb_start;
	unsigned char * image_buffer = jp->image_buffer;
	short image_bytes_per_pixel = jp->jpeg.num_components;
	unsigned long image_size = jp->image_size;

	if(screen_bits_per_pixel == 32){
		unsigned char *temp = malloc( screen_size  );
		//void *memset(void *s, int c, size_t n);
		memset(temp , 0 , screen_size / 4);

		if (temp == NULL)
		{
			fprintf(stderr , "malloc error\n" );
			exit(-1);

		}

		int a;
		int b;
		
		for( a = 0,b = 0 ; a < image_size  ; 
				a += image_bytes_per_pixel , b += screen_bits_per_pixel / 8){

			unsigned int color = c24_to_32(image_buffer[a] , 
											image_buffer[a+1] , 
											image_buffer[a+2]);
			
			temp[b] = getuc(color , 4);
			temp[b+1] = getuc(color , 3);
			temp[b+2] = getuc(color , 2);
			temp[b+3] = getuc(color , 1);
			
		}

		memcpy(fb_start , temp , screen_size);
		jp->image_buffer = temp;
		jp->image_size = jp->image_size / 3 * 4;
	}
}






unsigned short RGB888toRGB565 (unsigned char red, unsigned char green,
            unsigned char blue)
{

    unsigned short B = (blue >> 3) & 0x001F;
    unsigned short G = ((green >> 2) << 5) & 0x07E0;
    unsigned short R = ((red >> 3) << 11) & 0xF800;
    return (unsigned short) (R | G | B);

}
unsigned int c24_to_32 (unsigned short r, unsigned short g, unsigned short b)
{
	
    return (unsigned int) (0xFF000000 | ((unsigned short) r << 16) |
                 ((unsigned short) g << 8) | (unsigned short) b);
}

unsigned char getuc( unsigned int color, short i)
{
	switch(i)
	{
		case 1:
			
			return (unsigned char)(color >> 24);

		case 2:

			return (unsigned char)(color >> 16);

		case 3:

			return (unsigned char)(color >> 8);

		case 4:

			return (unsigned char)color;
		default : exit(1);
	}

}
void clear_frame_buffer( Tframebuffer *fb )
{
	
	memset( fb->fb_start , 0 , fb->screensize);

}
