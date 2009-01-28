EXEC = ftp
OBJ  = Ftp.o

all: $(EXEC)

ftp: $(OBJ)
	$(CC) $(LDFLAGS) -o $(EXECPATH)/$@ $(OBJ) -lsfml-network -lsfml-system

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXECPATH)/$(EXEC)
