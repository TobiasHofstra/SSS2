GCC = gcc
GCCFLAGS = -O3

TARGETS = \
			pp

HEADERS = \
			util.h

LIBS    = \
			-lmastik


all:		$(TARGETS)

pp:			pp.c
				$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS)


clean:
		rm -f $(TARGETS) *.o