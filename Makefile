.PHONY: all

all: build/CMakeCache.txt
	cd build && make -s

build:
	mkdir -p build

build/CMakeCache.txt: build
	cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake
