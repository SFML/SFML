SRC       = $(wildcard *.cpp)
SRCVORBIS = $(wildcard ./stb_vorbis/*.c)
OBJ       = $(SRC:.cpp=.o)
OBJVORBIS = $(SRCVORBIS:.c=.o)

ifeq ($(STATIC), yes)
    LIB     = libsfml-audio-s.a
    LIBNAME = $(LIBPATH)/$(LIB)
    INSTALL = 
else
    LIB     = libsfml-audio.so
    LIBNAME = $(LIBPATH)/$(LIB).$(VERSION)
    INSTALL = && $(LN) $(LNFLAGS) $(LIB).$(VERSION) $(DESTLIBDIR)/$(LIB)
endif

all: $(LIB)

libsfml-audio-s.a: $(OBJ) $(OBJVORBIS)
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJ) $(OBJVORBIS)

libsfml-audio.so: $(OBJ) $(OBJVORBIS)
	$(CPP) $(LDFLAGS) -Wl,-soname,$(LIB).$(VERSION) -o $(LIBNAME) $(OBJ) $(OBJVORBIS) -lsndfile -lopenal

$(OBJ): %.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

$(OBJVORBIS): %.o: %.c
	$(CC) -o $@ -c $< $(CFLAGSEXT)

.PHONY: clean mrproper

clean:
	@rm -rf $(OBJ) $(OBJVORBIS)

mrproper: clean
	@rm -rf $(LIBNAME)

install:
	@($(CP) $(LIBNAME) $(DESTLIBDIR) $(INSTALL))
