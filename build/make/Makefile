export SRCROOT    = ../../src/SFML
export DESTDIR    = /usr/local
export DESTLIBDIR = $(DESTDIR)/lib
export DESTINCDIR = $(DESTDIR)/include
export DESTDBGDIR = $(DESTLIBDIR)/debug/$(DESTLIBDIR)
export CC         = gcc
export CPP        = g++
export CFLAGS     = -W -Wall -pedantic -g -O2 -DNDEBUG -I../../include -I../../src -fPIC
export CFLAGSEXT  = -I../../include -I../../src -g -O2 -DNDEBUG -fPIC
export LDFLAGS    = -shared
export LIBPATH    = ../../lib
export VERSION    = 2.0
export CP         = cp
export LN         = ln
export LNFLAGS    = -s -f
export LIBS       = system window graphics audio network

all: sfml

sfml: $(LIBS)

examples:
	cd ../../examples/build/make && $(MAKE)

$(LIBS):
	mkdir -p $(LIBPATH)
	$(MAKE) -f Makefile.$@

install:
	mkdir -p $(DESTLIBDIR)
	mkdir -p $(DESTINCDIR)
	mkdir -p $(DESTDBGDIR)
	$(CP) -r ../../include/SFML/ $(DESTINCDIR)/
	find $(DESTINCDIR)/SFML -name .svn -type d -print0 | xargs -0 /bin/rm -rf
	$(MAKE) $@ -f Makefile.system
	$(MAKE) $@ -f Makefile.window
	$(MAKE) $@ -f Makefile.graphics
	$(MAKE) $@ -f Makefile.audio
	$(MAKE) $@ -f Makefile.network

clean mrproper:
	$(MAKE) $@ -f Makefile.system
	$(MAKE) $@ -f Makefile.window
	$(MAKE) $@ -f Makefile.graphics
	$(MAKE) $@ -f Makefile.audio
	$(MAKE) $@ -f Makefile.network
	cd ../../examples/build/make && $(MAKE) $@

.PHONY: clean mrproper

