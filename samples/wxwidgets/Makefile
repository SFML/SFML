EXEC = wxwidgets
OBJ  = Main.o wxSFMLCanvas.o

all: $(EXEC)

wxwidgets: $(OBJ)
	$(CC) $(LDFLAGS) -o $(EXECPATH)/$@ $(OBJ) -lsfml-graphics -lsfml-window -lsfml-system `wx-config --libs` `pkg-config --libs gtk+-2.0`

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS) -I/usr/include/gtk-2.0 `wx-config --cppflags` `pkg-config --cflags gtk+-2.0`

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXECPATH)/$(EXEC)
