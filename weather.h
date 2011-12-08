/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 25 16:04:19 CST			*
 ********************************************************
 *		after studying C 69 days		*
 *		after studying APUE 34 days		*
 ********************************************************/

#ifndef DB_WEATHER_H
#define DB_WEATHER_H
#include "framebuffer.h"
extern int h_errno;

int weather_getinfo(unsigned char *weatherinfo, int second);
int weather_nextline(int fd);
int weather_filter_weather(int fd, unsigned char *weatherinfo);
int weather_child_getinfo(unsigned char *weatherinfo);
void show_weather(Tframebuffer *fb , unsigned char * text);
void init_font();

#endif
