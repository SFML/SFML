export DEBUGBUILD = no
export STATIC     = no

ifeq ($(DEBUGBUILD), yes)
    DEBUGFLAGS = -g -DDEBUG
else
    DEBUGFLAGS = -O2 -DNDEBUG
endif

ifeq ($(STATIC), no)
    BUILDFLAGS = -fPIC
    LINKFLAGS  = -shared
else
    BUILDFLAGS = 
    LINKFLAGS  = 
endif

export CC         = gcc
export CPP        = g++
export CFLAGS     = -W -Wall -pedantic -I../../../include -I../../ $(DEBUGFLAGS) $(BUILDFLAGS)
export CFLAGSEXT  = -I../../../include -I../.. $(DEBUGFLAGS) $(BUILDFLAGS)
export LDFLAGS    = $(LINKFLAGS)
export LIBPATH    = ../../../lib
export VERSION    = 2.0export CP         = cp
export LN         = ln
export LNFLAGS    = -s -f
export AR         = ar
export ARFLAGS    = rcs
export DESTDIR    = /usr
export DESTLIBDIR = $(DESTDIR)/lib
export DESTINCDIR = $(DESTDIR)/include

all: sfml-system sfml-window sfml-network sfml-graphics sfml-audio

sfml-system:
	@(cd ./System && $(MAKE))

sfml-window:
	@(cd ./Window && $(MAKE))

sfml-network:
	@(cd ./Network && $(MAKE))

sfml-graphics:
	@(cd ./Graphics && $(MAKE))

sfml-audio:
	@(cd ./Audio && $(MAKE))

.PHONY: clean mrproper

clean:
	@(cd ./System && $(MAKE) $@	&& cd ../Window && $(MAKE) $@ && cd ../Network && $(MAKE) $@ && cd ../Graphics && $(MAKE) $@ && cd ../Audio && $(MAKE) $@)

mrproper: clean
	@(cd ./System && $(MAKE) $@	&& cd ../Window && $(MAKE) $@ && cd ../Network && $(MAKE) $@ && cd ../Graphics && $(MAKE) $@ && cd ../Audio && $(MAKE) $@)

install:
	@(mkdir -p $(DESTLIBDIR))
	@(mkdir -p $(DESTINCDIR))
	@(cd ./System && $(MAKE) $@	&& cd ../Window && $(MAKE) $@ && cd ../Network && $(MAKE) $@ && cd ../Graphics && $(MAKE) $@ && cd ../Audio && $(MAKE) $@ && $(CP) -r ../../../include/SFML/ $(DESTINCDIR)/)
