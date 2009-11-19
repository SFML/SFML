SRC = $(wildcard *.cpp ./Unix/*.cpp)
OBJ = $(SRC:.cpp=.o)

LIB         = libsfml-system.so
LIBNAME     = $(LIB).$(VERSION)
FULLLIBNAME = $(LIBPATH)/$(LIBNAME)
LINK        = $(LN) $(LNFLAGS) $(LIBNAME) $(DESTLIBDIR)/$(LIB)

all: $(LIB)

libsfml-system.so: $(OBJ)
	$(CPP) $(LDFLAGS) -Wl,-soname,$(LIBNAME) -o $(FULLLIBNAME) $(OBJ) -lpthread

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
