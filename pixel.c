#include "framebuffer.h"
#include "pixel.h"


/*
 * Set pixel attr
 */
int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
{
	point->x = x;
	point->y = y;
	point->color = color;
	
	return 0;
}

/*
 * Draw a pixel (x, y) at framebuffer with color
 */
int fb_draw_pixel(Tframebuffer *fbp, FB_POINT *point)
{
	unsigned char *p;	/* point framebuffer (x, y) pixel */
	unsigned long locate;

	/* over screen resolution */
	if(point->x < 0 || point->x >= fbp->fb_vinfo.xres){
		return 1;
	}
	if(point->y < 0 || point->y >= fbp->fb_vinfo.yres){
		return 1;
	}

	locate = (point->y * fbp->fb_vinfo.xres + point->x) 
		* fbp->fb_vinfo.bits_per_pixel / 8;
	
	p = fbp->fb_start + locate;

	*p++ = (point->color >> 0) & 0xff; /* blue */
	*p++ = (point->color >> 8) & 0xff; /* green */
	*p++ = (point->color >> 16) & 0xff; /* red */
	*p++ = (point->color >> 24) & 0x0; /* transp */

	return 0;
}

/*
 * Draw a pixel (x, y) at screen with color
 */
int fb_draw_pixel_screen(Tframebuffer *screenp, FB_POINT *point)
{
	unsigned char *p;	/* point screen (x, y) pixel */
	unsigned long locate;

	/* over screen resolution */
	if(point->x < 0 || point->x >= screenp->fb_vinfo.xres){
		return 1;
	}
	if(point->y < 0 || point->y >= screenp->fb_vinfo.yres){
		return 1;
	}

	locate = (point->y * screenp->fb_vinfo.xres + point->x) 
		* screenp->fb_vinfo.bits_per_pixel / 8;
	
	p =  screenp->fb_start + locate;

	*p++ = (point->color >> 0) & 0xff; /* blue */
	*p++ = (point->color >> 8) & 0xff; /* green */
	*p++ = (point->color >> 16) & 0xff; /* red */
	*p++ = (point->color >> 24) & 0x0; /* transp */

	return 0;
}

/*
 * Draw a pixel (x, y) at screen with color
 */
int fb_draw_pixel_screen_trans(Tframebuffer *screenp, FB_POINT *point)
{
	unsigned char *p;	/* point screen (x, y) pixel */
	unsigned long locate;
	unsigned char *q;

	/* over screen resolution */
	if(point->x < 0 || point->x >= screenp->fb_vinfo.xres){
		return 1;
	}
	if(point->y < 0 || point->y >= screenp->fb_vinfo.yres){
		return 1;
	}

	locate = (point->y * screenp->fb_vinfo.xres + point->x) 
		* (screenp->fb_vinfo.bits_per_pixel >> 3);
	
	p =  screenp->fb_start + locate;
	q = (unsigned char *)&point->color;

	p[3] = q[3];

	return 0;
}

int fb_set_pixel_trans(FB_POINT *point, int x, int y, unsigned char trans)
{
	unsigned char *p;
	
	point->x = x;
	point->y = y;

	p = (unsigned char *)&point->color;

	p[3] = trans;

	return 0;
}


unsigned long fb_formatRGB(unsigned char red, unsigned char green, unsigned char blue)
{
	unsigned long color = 0;

	color = (color << 0) | red;
	color = (color << 8) | green;
	color = (color << 8) | blue;
//	fprintf(stdout, "%#08lx\n", color);
	return color;
}
