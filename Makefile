# Authors: Js <js@gmail.com>
#  
# License GPLv3 or later, NO WARRANTY.


CC	= gcc
CFLAGS	= -Wall -I/usr/include/freetype2
LDFLAGS	= -lm -ljpeg -lpthread -lfreetype 

OBJECT	= $(wildcard *.c)
OBJECT_H = $(wildcard *.h)


all: photo.out

photo.out: $(OBJECT) $(OBJECT_H)
	$(CC) $(CFLAGS) -o $@ $(OBJECT)


.PHONY: clean

clean:
	rm -f *.o *~ photo.out
