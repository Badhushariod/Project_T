CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Isrc

all: build/device.exe build/test_port.exe build/test_input.exe

test: build/test_port.exe build/test_input.exe
	./build/test_port.exe
	./build/test_input.exe

run: build/device.exe
	./build/device.exe

clean:
	rm -rf build

.PHONY: all test run clean

build/device.exe: src/main.c src/port.c src/display.c src/port.h src/display.h
	mkdir -p build
	$(CC) $(CFLAGS) src/main.c src/port.c src/display.c -o build/device.exe
build/test_port.exe: tests/test_port.c src/port.c src/port.h
	mkdir -p build
	$(CC) $(CFLAGS) src/port.c tests/test_port.c -o build/test_port.exe

build/test_input.exe: tests/test_input.c src/input.c src/input.h
	mkdir -p build
	$(CC) $(CFLAGS) src/input.c tests/test_input.c -o build/test_input.exe
