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
An 8-LED output port and 8 active-low buttons, controlled from a command line.
Button 7 is the MODE button: it switches the device between three modes
(a state machine).

**LEDs directly:**
```
> set 2
PORT = 00000100   LEDs: [ . . . . . * . . ]
> read 2
LED 2 is ON
> set 9
error: no pin 9 (valid pins: 0-7)
```

**Buttons, through the firmware rules:**
```
> press 0                  (button 0 = light switch for LED 0)
PORT = 00000001   LEDs: [ . . . . . . . * ]
> tick 3                   (time passes, finger still down: no flicker)
PORT = 00000001   LEDs: [ . . . . . . . * ]
PORT = 00000001   LEDs: [ . . . . . . . * ]
PORT = 00000001   LEDs: [ . . . . . . . * ]
> release 0
PORT = 00000001   LEDs: [ . . . . . . . * ]
> press 1                  (button 1 = LED 1 on while held)
PORT = 00000011   LEDs: [ . . . . . . * * ]
> release 1
PORT = 00000001   LEDs: [ . . . . . . . * ]
```

**Modes (button 7):**
```
            press 7          press 7          press 7
  NORMAL ──────────► TEST ──────────► OFF ──────────► NORMAL ...
```
| Mode | LEDs | Buttons 0-6 |
|------|------|-------------|
| NORMAL (start-up) | driven by the rule table | work as shown above |
| TEST (lamp test) | all on | ignored |
| OFF | all off | ignored |

```
> mode
 Current mode is NORMAL
> press 7                  (NORMAL -> TEST: lamp test)
PORT = 11111111   LEDs: [ * * * * * * * * ]
> release 7
PORT = 11111111   LEDs: [ * * * * * * * * ]
> press 0                  (TEST ignores the other buttons)
PORT = 11111111   LEDs: [ * * * * * * * * ]
> release 0
PORT = 11111111   LEDs: [ * * * * * * * * ]
> press 7                  (TEST -> OFF)
PORT = 00000000   LEDs: [ . . . . . . . . ]
```

**Fault experiment:** `fault N` overwrites the mode with any number, the way
a memory bug would. The state machine recovers to the safe state (OFF, all
LEDs off) on the next press of button 7:
```
> fault 5
fault: mode overwritten with 5
> mode
 Current mode is Invalid
> press 7                  (unknown mode -> safe state)
PORT = 00000000   LEDs: [ . . . . . . . . ]
> mode
 Current mode is OFF
```
`fault` exists only for experiments; a real device would not have it.

Commands: `set N`, `clear N`, `toggle N`, `read N`, `press N`, `release N`,
`tick N`, `mode`, `fault N`, `show`, `help`, `quit`.

How a button press flows through the code:
```
press N (the "finger") -> input register -> mode_poll_all
    -> button_poll (edge detection) -> event
    -> mode_handle_event (state machine)
         - press of button 7: change mode (+ LED action)
         - other buttons, NORMAL only: rules_apply (rule table)
    -> port (LEDs) -> display
```
`press`/`release` only move the simulated pin. The firmware has to discover
the change itself, by polling, exactly as it would on a real board.

## Project layout
| Path | Purpose |
|------|---------|
| `src/port.c/.h` | LED output port: set/clear/toggle/read bits, rejects invalid pins |
| `src/input.c/.h` | button input port, active low (released = 1, pressed = 0) |
| `src/button.c/.h` | edge detection: turns levels into PRESSED / RELEASED events |
| `src/rules.c/.h` | rule table (array of structs): which button event drives which LED |
| `src/mode.c/.h` | state machine: NORMAL / TEST / OFF modes, recovery from a bad mode value |
| `src/display.c/.h` | prints the port as bits and as an LED picture |
| `src/main.c` | command line: reads commands, checks input, runs the firmware poll |
| `tests/` | 83 automated PASS/FAIL checks (port 19, input 17, button 12, rules 13, mode 22) |
| `exercises/` | small experiments from each lesson |
| `LEARNING_LOG.md` | what was learned in each milestone |

## Milestones
- [x] Milestone 0: Toolchain ready (4-stage build, error-message types)
- [x] Milestone 1: Simulated 8-LED output port (bit operations, validation, CLI, tests, make)
- [x] Milestone 2: Buttons (active-low input, edge detection, enum, struct rule table)
- [x] Milestone 3: Device modes (state machine with switch/enum, safe-state recovery, fault injection)
