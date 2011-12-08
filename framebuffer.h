#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__
#include <linux/fb.h>
typedef struct{
	char * file_name;
	struct fb_fix_screeninfo fb_finfo;
	struct fb_var_screeninfo fb_vinfo;
	unsigned long screensize; /* bytes */
	unsigned char *fb_start;    /* pointer mmap framebuffer starting */

}Tframebuffer;


Tframebuffer *init_frame_buffer(Tframebuffer *my_framebuffer, char * file_name);
#endif
