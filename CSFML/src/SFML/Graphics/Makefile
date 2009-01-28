LIB     = libcsfml-graphics.so
SRC     = $(wildcard *.cpp)
OBJ     = $(SRC:.cpp=.o)
LIBNAME = $(LIBPATH)/$(LIB).$(VERSION)

all: $(LIB)

libcsfml-graphics.so: $(OBJ)
	$(CPP) $(LDFLAGS) -Wl,-soname,$(LIB).$(VERSION) -o $(LIBNAME) $(OBJ) -lsfml-graphics -lsfml-window -lsfml-system

$(OBJ): %.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf $(OBJ)

mrproper: clean
	@rm -rf $(LIBNAME)

install:
	@($(CP) $(LIBNAME) $(DESTLIBDIR) && $(LN) $(LNFLAGS) $(DESTLIBDIR)/$(LIB).$(VERSION) $(DESTLIBDIR)/$(LIB))
