export CC         = gcc
export CPP        = g++
export CFLAGS     = -W -Wall -pedantic -Wno-unused -I../.. -I../../../include -g -O2 -DNDEBUG -fPIC -DCSFML_EXPORTS
export LDFLAGS    = -shared
export LIBPATH    = ../../../lib
export VERSION    = 2.0
export CP         = cp
export LN         = ln
export LNFLAGS    = -s -f
export DESTDIR    = /usr/local
export DESTLIBDIR = $(DESTDIR)/lib
export DESTINCDIR = $(DESTDIR)/include
export DESTDBGDIR = $(DESTLIBDIR)/debug/$(DESTLIBDIR)

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
	@(cd ./System && $(MAKE) $@	&& cd ../Window && $(MAKE) $@ && cd ../Network && $(MAKE) $@ && cd ../Graphics && $(MAKE) $@ && cd ../Audio && $(MAKE) $@ && $(CP) -r ../../../include/SFML/ $(DESTINCDIR) && find $(DESTINCDIR)/SFML -name .svn -type d -print0 | xargs -0 /bin/rm -rf)
