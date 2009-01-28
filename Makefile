
all: sfml

sfml:
	@(cd ./src/SFML && $(MAKE))

sfml-samples:
	@(cd ./samples && $(MAKE))

install:
	@(cd ./src/SFML && $(MAKE) $@)

clean:
	@(cd ./src/SFML && $(MAKE) $@)
	@(cd ./samples  && $(MAKE) $@)

mrproper:
	@(cd ./src/SFML && $(MAKE) $@)
	@(cd ./samples  && $(MAKE) $@) 
