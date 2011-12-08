/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 24 12:37:56 CST			*
 ********************************************************
 *		after studying C 68 days		*
 *		after studying APUE 33 days		*
 ********************************************************/

#ifndef FB_FONT_H
#define FB_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "framebuffer.h"

typedef struct font{
	FT_Library library;
	FT_Face face;
}FB_FONT;

int fb_font_open(char *fontname, FB_FONT *ffp);
int fb_font_close(FB_FONT *ffp);
int fb_font_set_charsize(FB_FONT *ffp, int fontsize);
int fb_font_draw_bitmap(Tframebuffer *screenp, FT_Bitmap*  bitmap, FT_Int x, FT_Int y);
#endif
