/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 24 12:38:26 CST			*
 ********************************************************
 *		after studying C 68 days		*
 *		after studying APUE 33 days		*
 ********************************************************/

#include "font.h"
#include "framebuffer.h"
#include "text.h"

/*
 * Show a string at (x, y) in screen
 */
int fb_text_show(Tframebuffer *screenp, char *str, FB_FONT *ffp, int x, int y)
{
	FT_GlyphSlot slot;
	FT_Error error;
	int n;
	int pen_x, pen_y;
	int num_chars;
	FT_UInt glyph_index;

	pen_x = x;
	pen_y = y;

	slot = ffp->face->glyph;

	num_chars = strlen(str);

	//int fb_font_draw_bitmap(FB_SCREEN *screenp, FT_Bitmap*  bitmap, FT_Int x, FT_Int y);
	for(n = 0; n < num_chars; n++){
		/* retrieve glyph index from character code*/
		glyph_index = FT_Get_Char_Index(ffp->face, str[n]);
			
		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph(ffp->face, glyph_index, FT_LOAD_DEFAULT);
		if(error){
			fprintf(stderr, "FT_Load_Glyph() failed: %d", error);
		}
		
		/* convert to an anti-aliased bitmap */
		error = FT_Render_Glyph(ffp->face->glyph, FT_RENDER_MODE_NORMAL); /* FIXME:slot */
		if(error){
			fprintf(stderr, "FT_Render_Glyph() failed: %d", error);
		}
		
		/* now, draw to our target surface */
		fb_font_draw_bitmap(screenp, &slot->bitmap, 
				    pen_x + slot->bitmap_left,
				    pen_y - slot->bitmap_top);
		
		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6;
	}
	
	
	return 0;
}

int fb_text_show_cn(Tframebuffer *screenp, unsigned char *str, FB_FONT *ffp, int x, int y)
{
	FT_GlyphSlot slot;
	FT_Error error;
	int offset;
	int pen_x, pen_y;
	FT_UInt glyph_index;

	pen_x = x;
	pen_y = y;

	slot = ffp->face->glyph;

	offset = 0;
	//int fb_font_draw_bitmap(FB_SCREEN *screenp, FT_Bitmap*  bitmap, FT_Int x, FT_Int y);
	while(str[offset] != '\0'){
		/* retrieve glyph index from character code*/
		glyph_index = FT_Get_Char_Index(ffp->face, 
						fb_text_utf8tolong(str, &offset));
			
		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph(ffp->face, glyph_index, FT_LOAD_DEFAULT);
		if(error){
			fprintf(stderr, "FT_Load_Glyph() failed: %d", error);
		}
		
		/* convert to an anti-aliased bitmap */
		error = FT_Render_Glyph(ffp->face->glyph, FT_RENDER_MODE_NORMAL); /* FIXME:slot */
		if(error){
			fprintf(stderr, "FT_Render_Glyph() failed: %d", error);
		}
		
		/* now, draw to our target surface */
		fb_font_draw_bitmap(screenp, &slot->bitmap, 
				    pen_x + slot->bitmap_left,
				    pen_y - slot->bitmap_top);
		
		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6;
	}
	
	return 0;
}

/*
 * change utf-8 to unsigned long
 */
unsigned long fb_text_utf8tolong(unsigned char *str, int *offset)
{
	unsigned long utflong;	/* return value */

	utflong = 0;

	if(str[*offset] < 0x80){	
		/* 1 unit */
		utflong |= str[*offset];
		*offset += 1;
	}else if(str[*offset] < 0xe0){
		/* 2 unit */
		utflong |= (str[*offset] & ~(0xe0));
		utflong <<= 7;

		utflong |= (str[*offset + 1] & ~(0xc0));

		utflong >>=2;

		*offset += 2;
	}else if(str[*offset] < 0xf0){
		/* 3 unit */
		utflong |= (str[*offset] & ~(0xe0));
		utflong <<= 8;

		utflong |= ((str[*offset + 1] & ~(0xc0)) << 2);
		utflong <<= 6;

		utflong |= ((str[*offset + 2] & ~(0xc0)) << 2);

		utflong >>= 2;

		*offset += 3;
	}
	
	return utflong;
}
