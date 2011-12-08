
#include <stdio.h>
#include "framebuffer.h"
#include "pixel.h"
#include "font.h"

int fb_font_open(char *fontname, FB_FONT *ffp)
{
	FT_Error error;

	error = FT_Init_FreeType(&ffp->library);
	if(error){
		fprintf(stderr, "FT_Init_FreeType failed\n");
		return -1;
	}
	
	error = FT_New_Face(ffp->library, fontname, 0, &ffp->face);

//	fprintf(stdout , "fontname=%s\n" , fontname);	

	if(error){
		fprintf(stderr, "FT_New_Face failed\n");
		return -1;
	}

	return 0;
}

int fb_font_close(FB_FONT *ffp)
{
	FT_Done_Face(ffp->face);
	FT_Done_FreeType(ffp->library);

	return 0;
}

int fb_font_set_charsize(FB_FONT *ffp, int fontsize)
{
	FT_Error error;

	error = FT_Set_Char_Size(ffp->face, 0, 16 * 64, fontsize, 0);
	if(error){
		fprintf(stderr, "FT_Set_Char_Size failed\n");
		return -1;
	}
		
	return 0;
}

int fb_font_draw_bitmap(Tframebuffer *screenp, FT_Bitmap *bitmap, FT_Int x, FT_Int y)
{
	COLOR_32 color[2], backcolor;
	FB_POINT point;

	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;
	unsigned char ch;

	color[0] = fb_formatRGB(255, 255, 255);
	color[1] = fb_formatRGB(120, 120, 120);
	backcolor = fb_formatRGB(0, 0, 0);
	
	for ( j = y, q = 0; j < y_max; j++, q++ )
	{
		for ( i = x, p = 0; i < x_max; i++, p++ ){
			ch = bitmap->buffer[q * bitmap->width + p];
			if(ch > 128){
				//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color);
				fb_set_pixel(&point, i, j, color[0]);
				//int fb_draw_pixel(struct framebuffer *fbp, FB_POINT *point);
				fb_draw_pixel_screen(screenp, &point);
			}else if(ch > 10){
				fb_set_pixel(&point, i, j, color[1]);
				fb_draw_pixel_screen(screenp, &point);
			}else{
				fb_set_pixel(&point, i, j, backcolor);
				fb_draw_pixel_screen(screenp, &point);
			}
		}
	}
	
	return 0;
}
