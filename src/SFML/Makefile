export CC         = gcc
export CPP        = g++
export CFLAGS     = -W -Wall -pedantic -g -O2 -DNDEBUG -I../../../include -I../../ -fPIC
export CFLAGSEXT  = -I../../../include -I../.. -g -O2 -DNDEBUG -fPIC
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

all: sfml-system sfml-window sfml-network sfml-graphics sfml-audio

sfml-system:
	@(mkdir -p ../../lib)
	@(cd ./System && $(MAKE))

sfml-window:
	@(mkdir -p ../../lib)
	@(cd ./Window && $(MAKE))

sfml-network:
	@(mkdir -p ../../lib)
	@(cd ./Network && $(MAKE))

sfml-graphics:
	@(mkdir -p ../../lib)
	@(cd ./Graphics && $(MAKE))

sfml-audio:
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
	@(mkdir -p $(DESTDBGDIR))
	@(cd ./System && $(MAKE) $@	&& cd ../Window && $(MAKE) $@ && cd ../Network && $(MAKE) $@ && cd ../Graphics && $(MAKE) $@ && cd ../Audio && $(MAKE) $@ && $(CP) -r ../../../include/SFML/ $(DESTINCDIR)/ && find $(DESTINCDIR)/SFML -name .svn -type d -print0 | xargs -0 /bin/rm -rf)
