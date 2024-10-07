dependencies = src/minesweeper.c src/captcha.c src/state.c

all: build-folder
	gcc -o build/main.out main.c $(dependencies)

raylib: build-folder
	gcc -o build/raylib raylib/main.c $(dependencies) -lraylib

build-folder:
	mkdir -p build

clean:
	rm -rf build
