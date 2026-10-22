CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Isrc

all: build/device.exe build/test_port.exe build/test_input.exe build/test_button.exe build/test_rules.exe

test: build/test_port.exe build/test_input.exe build/test_button.exe build/test_rules.exe
	./build/test_port.exe
	./build/test_input.exe
	./build/test_button.exe
	./build/test_rules.exe

run: build/device.exe
	./build/device.exe

clean:
	rm -rf build

.PHONY: all test run clean

build/device.exe: src/main.c src/port.c src/display.c src/input.c src/button.c src/rules.c src/port.h src/display.h src/input.h src/button.h src/rules.h
	mkdir -p build
	$(CC) $(CFLAGS) src/main.c src/port.c src/display.c src/input.c src/button.c src/rules.c -o build/device.exe

build/test_port.exe: tests/test_port.c src/port.c src/port.h
	mkdir -p build
	$(CC) $(CFLAGS) src/port.c tests/test_port.c -o build/test_port.exe

build/test_input.exe: tests/test_input.c src/input.c src/input.h
	mkdir -p build
	$(CC) $(CFLAGS) src/input.c tests/test_input.c -o build/test_input.exe

build/test_button.exe: tests/test_button.c src/button.c src/button.h src/input.c src/input.h
	mkdir -p build
	$(CC) $(CFLAGS) src/button.c src/input.c tests/test_button.c -o build/test_button.exe

build/test_rules.exe: tests/test_rules.c src/rules.c src/rules.h src/button.c src/button.h src/input.c src/input.h src/port.c src/port.h
	mkdir -p build
	$(CC) $(CFLAGS) src/rules.c src/button.c src/input.c src/port.c tests/test_rules.c -o build/test_rules.exe
