
all: sfml

sfml samples install clean mrproper:
	cd ./build/make && $(MAKE) $@

.PHONY: samples

