#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

#define FONT_PATH "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc"
int main(int argc , char **argv)
{
	
	
	Tframebuffer *fb = malloc(sizeof(Tframebuffer));
	fb = init_frame_buffer(fb , "/dev/fb0");
	
	FB_FONT *font = malloc(sizeof(FB_FONT));

    //int fb_font_open(char *fontname, FB_FONT *ffp);
	fb_font_open(FONT_PATH, font);
	
	//        //int fb_font_set_charsize(FB_FONT *ffp, int fontsize);
	fb_font_set_charsize(font, 150);

	fb_text_show_cn(fb, (unsigned char *)
		            "digit-box", font, 0, 30);



	// Tjpeg *get_image_buffer(Tjpeg *myjpeg)
	//Tjpeg *jp = malloc(sizeof(Tjpeg));
	//jp->file_name = "/home/jiangs/virusb/digitbox/jpg/001.jpg";
	//jp = get_image_buffer(jp);
	

	//void draw_to_screen(unsigned char * framebuffer , unsigned char * jpegbuffer )


	//jp = format_image_buffer(jp , 90 , 32);	

	//draw2(fb , jp);			

	//Tjpeg * format_image_buffer(Tjpeg *myjpeg , int perc , int bytes_per_pixel)
	
	
	//draw_to_screen_up_down(fb , jp);
	
	//Com_box  *img_search(char *dirname , Com_box * list)
	//Com_box *list = malloc(sizeof(Com_box));
	//list = img_search( "/home/jiangs/virusb/digitbox/jpg" , list);

	return 0;

}
