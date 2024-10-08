dependencies = src/core/minesweeper.c src/core/captcha.c src/core/state.c

all: build-folder
	gcc -o build/main.out main.c $(dependencies)

raylib: build-folder
	gcc -o build/raylib src/raylib.c $(dependencies) -lraylib

build-folder:
	mkdir -p build

clean:
	rm -rf build
