export SRCROOT  = ../..
export CPP      = g++
export CFLAGS   = -W -Wall -ansi -g -O2 -DNDEBUG -I../../include
export EXAMPLES = ftp opengl pong shader sockets sound sound_capture voip window X11

all: $(EXAMPLES)

$(EXAMPLES):
	$(MAKE) -f Makefile.$@

clean mrproper:
	for example in $(EXAMPLES); do $(MAKE) $@ -f Makefile.$${example}; done

.PHONY: clean mrproper

