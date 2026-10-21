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

## Build, run and test (from the project root)
All commands run in the **MSYS2 UCRT64** shell. If your terminal prompt starts
with `PS` (PowerShell) or `C:\>` (cmd), type `./msys` (or `msys` in cmd)
first to switch into it.

| Command      | What it does                                   |
|--------------|------------------------------------------------|
| `make`       | build the simulator and the tests              |
| `make run`   | build if needed, then start the LED simulator  |
| `make test`  | build if needed, then run the automated tests  |
| `make clean` | delete the `build` folder                      |

The code builds with **zero warnings** under `-std=c11 -Wall -Wextra -Wpedantic`.

## The simulated device
An 8-bit output port driving 8 LEDs, controlled from a command line:
```
> set 2
PORT = 00000100   LEDs: [ . . . . . * . . ]
> toggle 0
PORT = 00000101   LEDs: [ . . . . . * . * ]
> read 2
LED 2 is ON
> set 9
error: no pin 9 (valid pins: 0-7)
```
Commands: `set N`, `clear N`, `toggle N`, `read N`, `show`, `help`, `quit`.

## Project layout
| Path | Purpose |
|------|---------|
| `src/port.c`, `src/port.h` | port "driver": set/clear/toggle/read bits, rejects invalid pins |
| `src/display.c`, `src/display.h` | prints the port as bits and as an LED picture |
| `src/main.c` | command line: reads commands, checks input, calls the port |
| `tests/test_port.c` | 19 automated PASS/FAIL checks for the port module |
| `exercises/` | small experiments from each lesson |
| `LEARNING_LOG.md` | what was learned in each milestone |

## Milestones
- [x] Milestone 0: Toolchain ready (4-stage build, error-message types)
- [x] Milestone 1: Simulated 8-LED output port (bit operations, validation, CLI, tests, make)
