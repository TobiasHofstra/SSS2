GCC = gcc
GCCFLAGS = -O3

TARGETS = \
			pp

HEADERS = \
			util.h

all:		$(TARGETS)

pp:			pp.c
				$(GCC) $(GCCFLAGS) -o $@ $^

clean:
		rm -f $(TARGETS) *.o