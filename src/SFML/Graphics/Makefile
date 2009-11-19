SRC     = $(wildcard *.cpp ./Linux/*.cpp)
SRCGLEW = $(wildcard ./GLEW/*.c)
SRCJPEG = $(wildcard ./libjpeg/*.c)
SRCPNG  = $(wildcard ./libpng/*.c)
SRCSOIL = $(wildcard ./SOIL/*.c)
SRCZLIB = $(wildcard ./zlib/*.c)
OBJ     = $(SRC:.cpp=.o)
OBJGLEW = $(SRCGLEW:.c=.o)
OBJJPEG = $(SRCJPEG:.c=.o)
OBJPNG  = $(SRCPNG:.c=.o)
OBJSOIL = $(SRCSOIL:.c=.o)
OBJZLIB = $(SRCZLIB:.c=.o)

LIB         = libsfml-graphics.so
LIBNAME     = $(LIB).$(VERSION)
FULLLIBNAME = $(LIBPATH)/$(LIBNAME)
LINK        = $(LN) $(LNFLAGS) $(LIBNAME) $(DESTLIBDIR)/$(LIB)

all: $(LIB)

libsfml-graphics.so: $(OBJ) $(OBJGLEW) $(OBJJPEG) $(OBJPNG) $(OBJSOIL) $(OBJZLIB)
	$(CPP) $(LDFLAGS) -Wl,-soname,$(LIBNAME) -o $(FULLLIBNAME) $(OBJ) $(OBJGLEW) $(OBJJPEG) $(OBJPNG) $(OBJSOIL) $(OBJZLIB) -lfreetype -lX11 -lGL

$(OBJ): %.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS) -I/usr/include/freetype2

$(OBJSOIL): %.o: %.c
	$(CC) -o $@ -c $< $(CFLAGSEXT) -DSTBI_FAILURE_USERMSG

$(OBJGLEW) $(OBJJPEG) $(OBJPNG) $(OBJZLIB): %.o: %.c
	$(CC) -o $@ -c $< $(CFLAGSEXT)

.PHONY: clean mrproper

clean:
	@rm -rf $(OBJ) $(OBJGLEW) $(OBJJPEG) $(OBJPNG) $(OBJSOIL) $(OBJZLIB)

mrproper: clean
	@rm -rf $(FULLLIBNAME)

install:
	objcopy --only-keep-debug $(FULLLIBNAME) $(DESTDBGDIR)/$(LIBNAME)
	objcopy --strip-unneeded  $(FULLLIBNAME) $(DESTLIBDIR)/$(LIBNAME)
	$(LINK)
