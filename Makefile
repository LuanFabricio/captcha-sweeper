dependencies = $(shell find src/core/ -type f -name '*.c')

wasm_flags = --target=wasm32 --no-standard-libraries -Wl,--export-table -Wl,--no-entry -Wl,--allow-undefined -Wl,--export=main

all: build-folder
	gcc -o build/main.out main.c $(dependencies)

raylib: build-folder
	gcc -g -o build/raylib src/platform_raylib.c $(dependencies) -lraylib

wasm: build-folder
	clang $(wasm_flags) -o build/main.wasm $(dependencies) -DWEB
	cp build/main.wasm web/main.wasm

build-folder:
	mkdir -p build

clean:
	rm -rf build
