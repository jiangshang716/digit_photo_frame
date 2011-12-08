#ifndef __JPEG_H__
#define __JPEG_H__
#include <jpeglib.h>
#include "framebuffer.h"
typedef struct {
	char *file_name;
	struct jpeg_decompress_struct jpeg;
	struct jpeg_error_mgr jerr;
	unsigned char *image_buffer;
	unsigned long image_size;
}Tjpeg;


Tjpeg *get_image_buffer(Tjpeg *myjpeg);

Tjpeg * format_image_buffer(Tjpeg *myjpeg , int perc , int bytes_per_pixel);

void draw2(Tframebuffer *fb , Tjpeg *jp);
#endif

