export CC         = gcc
export CPP        = g++
export CFLAGS     = -W -Wall -pedantic -fPIC -Wno-unused -I../.. -I../../../include -DNDEBUG -DCSFML_EXPORTS -O2
export LDFLAGS    = -shared
export LIBPATH    = ../../../lib
export VERSION    = 1.6
export CP         = cp
export LN         = ln
export LNFLAGS    = -s -f
export DESTDIR    = /usr/local
export DESTLIBDIR = $(DESTDIR)/lib
export DESTINCDIR = $(DESTDIR)/include

all: csfml-system csfml-window csfml-network csfml-graphics csfml-audio

csfml-system:
	@(mkdir -p ../../lib)
	@(cd ./System && $(MAKE))

csfml-window:
	@(mkdir -p ../../lib)
	@(cd ./Window && $(MAKE))

csfml-network:
	@(mkdir -p ../../lib)
	@(cd ./Network && $(MAKE))

csfml-graphics:
	@(mkdir -p ../../lib)
	@(cd ./Graphics && $(MAKE))

csfml-audio:
	@(mkdir -p ../../lib)
	@(cd ./Audio && $(MAKE))

.PHONY: clean mrproper

clean:
	@(cd ./System && $(MAKE) $@	&& cd ../Window && $(MAKE) $@ && cd ../Network && $(MAKE) $@ && cd ../Graphics && $(MAKE) $@ && cd ../Audio && $(MAKE) $@)

mrproper: clean
	@(cd ./System && $(MAKE) $@	&& cd ../Window && $(MAKE) $@ && cd ../Network && $(MAKE) $@ && cd ../Graphics && $(MAKE) $@ && cd ../Audio && $(MAKE) $@)

install:
	@(mkdir -p $(DESTLIBDIR))
	@(mkdir -p $(DESTINCDIR))
	@(cd ./System && $(MAKE) $@	&& cd ../Window && $(MAKE) $@ && cd ../Network && $(MAKE) $@ && cd ../Graphics && $(MAKE) $@ && cd ../Audio && $(MAKE) $@ && $(CP) -r ../../../include/SFML/ $(DESTINCDIR))
