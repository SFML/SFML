export SRCROOT = ../..
export BINPATH = ../../bin
export CPP     = g++
export CFLAGS  = -W -Wall -ansi -g -O2 -DNDEBUG -I../../include
export SAMPLES = ftp opengl pong qt shader sockets sound sound_capture voip window wxwidgets X11

all: $(SAMPLES)

$(SAMPLES):
	mkdir -p $(BINPATH)
	$(MAKE) -f Makefile.$@

clean mrproper:
	for sample in $(SAMPLES); do $(MAKE) $@ -f Makefile.$${sample}; done

.PHONY: clean mrproper

