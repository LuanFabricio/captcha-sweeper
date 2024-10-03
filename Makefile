all: build-folder
	gcc -o build/main.out main.c src/minesweeper.c

build-folder:
	mkdir -p build

clean:
	rm -rf build
