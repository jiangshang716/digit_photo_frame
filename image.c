#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <jpeglib.h>
#include "image.h"

Tjpeg * format_image_buffer(Tjpeg *myjpeg , int perc , int bytes_per_pixel)
{
//#if __DEBUG__
	
	fprintf(stdout , "perc=%d\n" , perc );
	fprintf(stdout , "bytes_er=%d\n" , bytes_per_pixel );

//#endif	

	printf("d\n");	
	int image_width = myjpeg->jpeg.image_width * perc / 100;
	int image_height = myjpeg->jpeg.image_height * perc / 100;
	unsigned long n_image_size = image_width * image_height * bytes_per_pixel / 8;

	unsigned long o_image_size = myjpeg->image_size;
	unsigned char *old_buffer = myjpeg->image_buffer;
	// get remove steps
	int step = ( myjpeg->jpeg.image_width ) / (  myjpeg->jpeg.image_width - image_width) - 1;

	unsigned char *temp = (unsigned char *)malloc( n_image_size );


	unsigned long line_begin = ( myjpeg->jpeg.image_width * bytes_per_pixel + 1 ) * step;
	unsigned long line_end =  ( myjpeg->jpeg.image_width * bytes_per_pixel + 1 ) * step + myjpeg->jpeg.image_width*bytes_per_pixel;

	
	int column = 0;
	int b = 0;	
	int i;
//#if __DEBUG__
	
	fprintf(stdout , "new image_width=%d\n" , image_width );
	fprintf(stdout , "new image_height=%d\n" , image_height );
	fprintf(stdout , "new image_size=%ld\n" , n_image_size );
	fprintf(stdout , "old image_size=%ld\n" , o_image_size );
	fprintf(stdout , "step=%d\n" , step );

//#endif	
//	delete line and column
	for(i = 0 ; i < o_image_size ; i ++ )
	{
	//	printf("d\n");
		if( ( i >= line_begin &&  i <= line_end ) || column == step - 1 )
		{
			column = 0;
			if(i == line_end)
			{

				line_begin += myjpeg->jpeg.image_width * bytes_per_pixel * step;
				line_end += myjpeg->jpeg.image_width * bytes_per_pixel * step;
			}
			continue;
		}
		temp[b] = old_buffer[i];
	//	memcpy (temp , &old_buffer[i] , 1);
	//	printf("%0x  \n" , temp[b]);
		printf("%0x  \n" , old_buffer[i]);
		
		sleep(1);
		temp++;// old_buffer++;
		column ++;
		b ++;
			
	}		

	myjpeg->jpeg.image_width = image_width;
	myjpeg->jpeg.image_height = image_height;
	myjpeg->image_size = n_image_size;	
	myjpeg->image_buffer = temp;
	
	return myjpeg;
}



void draw2(Tframebuffer *fb , Tjpeg *jp)
{

	unsigned char *frame_buffer = fb->fb_start;
	int fb_width = fb->fb_vinfo.xres;
	//int fb_heigth = fb->fb_vinfo.yres;
	unsigned long write_size = jp->image_size;
	
	unsigned char* image_buffer = jp->image_buffer;
	int image_width = jp->jpeg.image_width;
	//int image_height = jp->jpeg.image_height;


	int i;
	int y = 0;
	int x = 0;
	for( i = 0 ; i < write_size ; i ++)
	{
		if( (( i + 1) % image_width) == 0 )
		{
			y ++;
			x = 0;	
		}
		memcpy(frame_buffer + y * fb_width +  x, &image_buffer[i] , 1 );
	}
		
}



#if 0
	typedef struct {
		char *file_name;
		struct jpeg_decompress_struct jpeg;
		char *buffer;
	}Tjpeg;
#endif
Tjpeg *get_image_buffer(Tjpeg *myjpeg)
{
	FILE *fd_image;
	
	if(((fd_image = fopen(myjpeg->file_name , "rb"))) == NULL)
	{
		fprintf(stderr , "open %s failed:%s\n" , myjpeg->file_name , strerror(errno));
		exit(1);	
	}
//	fprintf(stdout , "image %s open success\n" , myjpeg->file_name);
	
	// init jpeg decompress object error handler
	myjpeg->jpeg.err = jpeg_std_error(&myjpeg->jerr);
	jpeg_create_decompress(&myjpeg->jpeg);
	// bind jpeg decompress object to image_file	
	jpeg_stdio_src(&myjpeg->jpeg , fd_image);
	// read jpeg header
	jpeg_read_header(&myjpeg->jpeg , TRUE);
  /*
   * decompress process.
   * note: after jpeg_start_decompress() is called
   * the dimension infomation will be known,
   * so allocate memory buffer for scanline immediately
   */	
	
//	fprintf(stdout , "width=%d  heigth=%d  components=%d\n", myjpeg->jpeg.image_width , myjpeg->jpeg.image_height , myjpeg->jpeg.num_components);	
	
	myjpeg->image_size = myjpeg->jpeg.image_width * myjpeg->jpeg.image_height * myjpeg->jpeg.num_components;

	if(myjpeg->jpeg.num_components * 8 == 16 || myjpeg->jpeg.num_components * 8 == 32)
	{
		fprintf(stderr , "can not deal with 16bits and 32bits picture name:%s\n" , myjpeg->file_name);
		exit(1);
	}	
	
	unsigned char *image_buffer = (unsigned char *)malloc(myjpeg->image_size);
//	printf("iamge_size=%ld\n" , myjpeg->image_size);
	if(image_buffer == NULL)
	{
		fprintf( stderr , "image_buffer malloc error %s\n" , __func__);
		exit(1);
	}
//	printf("d");
//	unsigned char *o_image_buffer = image_buffer;
	JSAMPROW row_pointer[1]; 
    row_pointer[0] = (unsigned char *)malloc(myjpeg->jpeg.image_width * myjpeg->jpeg.num_components);
	//unsigned char *temp_buffer = (unsigned char *)malloc(myjpeg->jpeg.image_width * myjpeg->jpeg.num_components);

	if(row_pointer[0] == NULL)
	{
		fprintf( stderr , "temp_buffer malloc error %s\n" , __func__);
		exit(1);
	}
	
	jpeg_start_decompress(&myjpeg->jpeg);

	unsigned long current_idx = 0;
	int i;

	while( myjpeg->jpeg.output_scanline < myjpeg->jpeg.image_height )
	{
		
//		fprintf(stdout  ,"%ld " , current_idx);

		jpeg_read_scanlines(&myjpeg->jpeg ,  row_pointer  , 1);
		for ( i = 0 ; i < myjpeg->jpeg.image_width * myjpeg->jpeg.num_components ; i ++)
		{
			image_buffer[current_idx++] = row_pointer[0][i];
//			current_idx ++;

		}

	}

	jpeg_finish_decompress(&myjpeg->jpeg);
	jpeg_destroy_decompress(&myjpeg->jpeg);

	myjpeg->image_buffer = image_buffer;
	free(row_pointer[0]);
	
	fclose(fd_image);	

	return myjpeg;			
}	
