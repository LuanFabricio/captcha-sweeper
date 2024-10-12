dependencies = $(shell find src/core/ -type f -name '*.c')

all: build-folder
	gcc -o build/main.out main.c $(dependencies)

raylib: build-folder
	gcc -o build/raylib src/raylib.c $(dependencies) -lraylib

build-folder:
	mkdir -p build

clean:
	rm -rf build
