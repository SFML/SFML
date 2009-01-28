EXEC = sound
OBJ  = Sound.o

all: $(EXEC)

sound: $(OBJ)
	$(CC) $(LDFLAGS) -o $(EXECPATH)/$@ $(OBJ) -lsfml-audio -lsfml-system

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXECPATH)/$(EXEC)
