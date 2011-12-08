#ifndef __DRAW_H__
#define __DRAW_H__
#include "framebuffer.h"
#include "image.h"

void draw_to_screen(Tframebuffer *framebuffer ,Tjpeg *jpeg);
void draw_to_screen_begin_line(Tframebuffer *framebuffer ,Tjpeg *jpeg , int begin);
void draw_to_screen_up_to_down(Tframebuffer *framebuffer ,Tjpeg *jpeg , int begin);
void draw_to_screen_down_to_up( Tframebuffer *fb , Tjpeg *jp ,int begin);
void draw_to_screen_left_to_right( Tframebuffer *fb , Tjpeg *jp ,int begin);
void draw_to_screen_right_to_left( Tframebuffer *fb , Tjpeg *jp ,int begin);
void draw_to_screen_up_to_down_plus( Tframebuffer *fb , Tjpeg *jp ,int begin);
void clear_frame_buffer( Tframebuffer *fb );

unsigned short RGB888toRGB565 (unsigned char red, unsigned char green, unsigned char bule);
unsigned int c24_to_32 (unsigned short r, unsigned short g, unsigned short b);
unsigned char *get_format_to_32_jpg(  Tframebuffer *fb , Tjpeg *jp  );
unsigned char getuc( unsigned int color, short i);
void *auto_display_pthread_fun();
pthread_t start_auto_display_pthread();
#endif
