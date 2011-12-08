#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <string.h>
#include "framebuffer.h"


Tframebuffer *init_frame_buffer(Tframebuffer *my_framebuffer , char * file_name)
{

	int fd_framebuffer;

	if((fd_framebuffer = open(file_name , O_RDWR)) < 0)
	{
		// failed
		fprintf(stderr , "framebuffer %s open error :%s" , my_framebuffer->file_name , strerror(errno));
		exit(-1);
	}
	//	fprintf(stdout , "fd_framebuffer=%d\n" , fd_framebuffer);
	//	int ioctl(int d, int request, ...);
    if(ioctl(fd_framebuffer, FBIOGET_FSCREENINFO, &my_framebuffer->fb_finfo) < 0){
        fprintf(stderr, "get fixed screen information failed: %s\n",
            strerror(errno));
		exit(-1);    
	}
    if(ioctl(fd_framebuffer, FBIOGET_VSCREENINFO, &my_framebuffer->fb_vinfo) < 0){
        fprintf(stderr, "get var screen information failed: %s\n",
            strerror(errno));
        exit(-1);
    }

	my_framebuffer->screensize = my_framebuffer->fb_vinfo.xres * my_framebuffer->fb_vinfo.yres * 
			my_framebuffer->fb_vinfo.bits_per_pixel / 8 ;	

	my_framebuffer->fb_start = mmap(NULL, my_framebuffer->screensize, PROT_READ
                | PROT_WRITE, MAP_SHARED, fd_framebuffer, 0);
#if __DEBUG__
	fprintf(stdout , "framebuffer width=%d\n" , my_framebuffer->fb_vinfo.xres);
	fprintf(stdout , "framebuffer height=%d\n" , my_framebuffer->fb_vinfo.yres);
	fprintf(stdout , "framebuffer pixel=%d\n" , my_framebuffer->fb_vinfo.bits_per_pixel);
#endif

	close(fd_framebuffer);
	return my_framebuffer; 
}

