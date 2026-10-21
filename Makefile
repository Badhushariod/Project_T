CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Isrc


build/device.exe: src/main.c src/port.c src/display.c src/port.h src/display.h
	mkdir -p build
	$(CC) $(CFLAGS) src/main.c src/port.c src/display.c -o build/device.exe
build/test_port.exe: tests/test_port.c src/port.c src/port.h
	mkdir -p build
	$(CC) $(CFLAGS) src/port.c tests/test_port.c -o build/test_port.exe
