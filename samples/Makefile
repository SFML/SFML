export CC       = g++
export CFLAGS   = -W -Wall -ansi -g -O2 -DNDEBUG -I../../include
export LDFLAGS  = 
export EXECPATH = ../bin

all: ftp-sample opengl-sample pong-sample qt-sample shader-sample sockets-sample sound-sample sound_capture-sample voip-sample window-sample wxwidgets-sample X11-sample

ftp-sample:
	@(mkdir -p bin)
	@(cd ./ftp && $(MAKE))

opengl-sample:
	@(mkdir -p bin)
	@(cd ./opengl && $(MAKE))

pong-sample:
	@(mkdir -p bin)
	@(cd ./pong && $(MAKE))

shader-sample:
	@(mkdir -p bin)
	@(cd ./shader && $(MAKE))

qt-sample:
	@(mkdir -p bin)
	@(cd ./qt && $(MAKE))

sockets-sample:
	@(mkdir -p bin)
	@(cd ./sockets && $(MAKE))

sound-sample:
	@(mkdir -p bin)
	@(cd ./sound && $(MAKE))

sound_capture-sample:
	@(mkdir -p bin)
	@(cd ./sound_capture && $(MAKE))

voip-sample:
	@(mkdir -p bin)
	@(cd ./voip && $(MAKE))

window-sample:
	@(mkdir -p bin)
	@(cd ./window && $(MAKE))

wxwidgets-sample:
	@(mkdir -p bin)
	@(cd ./wxwidgets && $(MAKE))

X11-sample:
	@(mkdir -p bin)
	@(cd ./X11 && $(MAKE))

.PHONY: clean mrproper

clean:
	@(cd  ./ftp           && $(MAKE) $@ && \
	  cd ../opengl        && $(MAKE) $@ && \
	  cd ../pong          && $(MAKE) $@ && \
	  cd ../qt            && $(MAKE) $@ && \
	  cd ../shader        && $(MAKE) $@ && \
	  cd ../sockets       && $(MAKE) $@ && \
	  cd ../sound         && $(MAKE) $@ && \
	  cd ../sound_capture && $(MAKE) $@ && \
	  cd ../voip          && $(MAKE) $@ && \
	  cd ../window        && $(MAKE) $@ && \
	  cd ../wxwidgets     && $(MAKE) $@ && \
	  cd ../X11           && $(MAKE) $@)

mrproper: clean
	@(cd  ./ftp           && $(MAKE) $@ && \
	  cd ../opengl        && $(MAKE) $@ && \
	  cd ../pong          && $(MAKE) $@ && \
	  cd ../qt            && $(MAKE) $@ && \
	  cd ../shader        && $(MAKE) $@ && \
	  cd ../sockets       && $(MAKE) $@ && \
	  cd ../sound         && $(MAKE) $@ && \
	  cd ../sound_capture && $(MAKE) $@ && \
	  cd ../voip          && $(MAKE) $@ && \
	  cd ../window        && $(MAKE) $@ && \
	  cd ../wxwidgets     && $(MAKE) $@ && \
	  cd ../X11           && $(MAKE) $@)
