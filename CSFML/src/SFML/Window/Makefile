LIB         = libcsfml-window.so
SRC         = $(wildcard *.cpp)
OBJ         = $(SRC:.cpp=.o)
LIBNAME     = $(LIB).$(VERSION)
FULLLIBNAME = $(LIBPATH)/$(LIBNAME)
LINK        = $(LN) $(LNFLAGS) $(LIBNAME) $(DESTLIBDIR)/$(LIB)

all: $(LIB)

libcsfml-window.so: $(OBJ)
	$(CPP) $(LDFLAGS) -Wl,-soname,$(LIBNAME) -o $(FULLLIBNAME) $(OBJ) -lsfml-window -lsfml-system

$(OBJ): %.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf $(OBJ)

mrproper: clean
	@rm -rf $(FULLLIBNAME)

install:
	objcopy --only-keep-debug $(FULLLIBNAME) $(DESTDBGDIR)/$(LIBNAME)
	objcopy --strip-unneeded  $(FULLLIBNAME) $(DESTLIBDIR)/$(LIBNAME)
	$(LINK)
