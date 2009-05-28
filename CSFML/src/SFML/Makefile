export CC         = gcc
export CPP        = g++
export CFLAGS     = -W -Wall -pedantic -fPIC -Wno-unused -I../.. -I../../../include -DNDEBUG -DCSFML_EXPORTS -O2
export LDFLAGS    = -shared
export LIBPATH    = ../../../lib
export VERSION    = 1.6
export CP         = cp
export LN         = ln
export LNFLAGS    = -s -f
export DESTDIR    = /usr
export DESTLIBDIR = $(DESTDIR)/lib
export DESTINCDIR = $(DESTDIR)/include

all: csfml-system csfml-window csfml-network csfml-graphics csfml-audio

csfml-system:
	@(cd ./System && $(MAKE))

csfml-window:
	@(cd ./Window && $(MAKE))

csfml-network:
	@(cd ./Network && $(MAKE))

csfml-graphics:
	@(cd ./Graphics && $(MAKE))

csfml-audio:
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
