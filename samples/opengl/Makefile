EXEC = opengl
OBJ  = OpenGL.o

all: $(EXEC)

opengl: $(OBJ)
	$(CC) $(LDFLAGS) -o $(EXECPATH)/$@ $(OBJ) -lsfml-graphics -lsfml-window -lsfml-system -lGLU -lGL

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXECPATH)/$(EXEC)
