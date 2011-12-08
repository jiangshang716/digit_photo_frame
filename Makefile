# Authors: Js <js@gmail.com>
#  
# License GPLv3 or later, NO WARRANTY.


CC	?= gcc
CFLAGS	:= $(CFLAGS) -Wall
LDFLAGS	+= -lm -ljpeg -lpthread -lfreetype 

OBJECTS	+= $(patsubst %.c,%.o,$(wildcard *.c))

all: photo.out

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.d:%.c
	@set -e; \
	  rm -f $@; \
          $(CC) -M -MM $(CFLAGS) $< > $@.$$$$; \
          sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
          rm -f $@.$$$$

-include $(patsubst %.c,%.d,$(wildcard *.c))

photo.out: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^


.PHONY: clean

clean:
	rm -f *.o *.d *~ photo.out
