all: build-folder
	gcc -o build/main.out main.c src/minesweeper.c

raylib: build-folder
	gcc -o build/raylib raylib/main.c src/minesweeper.c -lraylib

build-folder:
	mkdir -p build

clean:
	rm -rf build
