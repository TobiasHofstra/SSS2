GCC = gcc
GCCFLAGS = -O3 -g


TARGETS = \
			pp

HEADERS = \
			util.h

LIBS    = \
			-lmastik

help:
	@echo "Use":
	@echo ""
	@echo "$$ make all" 
	@echo ""
	@echo "to compile all attacks (requires the OpenSSL crypto library), or"
	@echo ""
	@echo "$$ make <attack1> <attack2> ... <attack3>"
	@echo ""
	@echo "to compile the specific attacks you want."



all:		$(TARGETS)

pp:			pp.c
				$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) -lcrypto

clean:
		rm -f $(TARGETS) *.o