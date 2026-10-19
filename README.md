# Project_T: Embedded Systems Learning Lab

A simulated embedded device written in C, built milestone by milestone
to learn firmware fundamentals.

## Requirements

### Windows
1. Install MSYS2 (https://www.msys2.org) to `C:\msys64`.
2. Open the **MSYS2 UCRT64** terminal and install the toolchain:
   ```
   pacman -Syu
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb make
   ```
3. Open this folder in VS Code. `.vscode/settings.json` makes **MSYS2 UCRT64**
   the default terminal for this project only.

### Linux
Install `gcc`, `gdb`, `make` and `git` from your package manager.

### Verified toolchain
GCC 16.2.0 (MSYS2 UCRT64), GDB 18.1, GNU Make 4.4.1. Other recent versions
should work. If a build behaves differently, note your version in
`LEARNING_LOG.md`.

## Build and run (from the project root)
All commands run in the **MSYS2 UCRT64** shell. If your terminal prompt starts
with `PS` (PowerShell) or `C:\>` (cmd), type `./msys` (or `msys` in cmd)
first to switch into it.
```
mkdir -p build
gcc -std=c11 -Wall -Wextra -Wpedantic exercises/00_toolchain/hello.c -o build/hello.exe
./build/hello.exe
```
Expected output: `Project_T: toolchain OK`

## Current milestone
Milestone 0: Toolchain ready
