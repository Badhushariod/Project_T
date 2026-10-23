# Learning Log

## Current status
- Milestone: 3 in progress (device modes: a state machine)
- GCC version: 16.2.0 (MSYS2 UCRT64)
- Next task: Lesson 3.1, state machine concept and warm-up exercise

## Starting assessment (2026-10-19)
Background: Nuvoton, ESP, Keil. I know ISRs, direct register access and
scatter files as ideas. At work I have mostly written firmware with AI help.
Goal of this project: be able to write and debug it on my own.

| Topic | Result | Notes |
|---|---|---|
| Integer promotion (`d = a + b`) | ✅ Knew it | |
| `uint8_t` range and wrap-around | ✅ After hints | I said the max was 247; it's 255. 300 → 44, 260 → 4. Checked with `exercises/01_integers/q1.c` |
| Signed vs unsigned compare | ✅ After hints | `-1` becomes 4294967295, so `-1 < 1u` is false |
| Pointer arithmetic | ✅ With a picture | `p += 2` moves 2 elements; `*p` writes into `arr` |
| Array passed to a function | ✅ Knew it | It becomes a pointer, so `sizeof` gives the pointer size (4 on an MCU, 8 on a PC) |
| Bit operations | ⚠️ Had forgotten | Relearned set/clear/toggle/check. I mixed up the AND/OR/XOR rules, so this is the focus of Milestone 1 |
| Struct padding | ✅ With guidance | 12 bytes; ordering members largest first gives 8 |
| `static` local vs normal local | ✅ | I swapped the outputs at first. `static` = created once and remembered; it does NOT mean constant |
| Returning a pointer to a local | ❌ Didn't know | Dangling pointer: the stack memory is gone after return. Revise in the memory stage |
| `volatile` | ✅ Knew the idea | It forces a memory read every time; it does NOT make shared data safe |
| Race conditions | ❌ Not yet | `count++` is read, add, write, and an ISR can cut in between. Comes in the concurrency stage |
| Tools | New | First command-line build today. Makefile and GDB not used yet |

## Milestone 0: Toolchain ready (completed 2026-10-19) ✅
### Concepts introduced
- The 4 build stages: preprocessor (`-E`, .c → .i), compiler (`-S`, .i → .s),
  assembler (`-c`, .s → .o), linker (.o → .exe)
- `#include` = copy-paste a whole file (9 lines → 1791 lines)
- `#define` = find-and-replace before the compiler runs; the compiler never sees the name
- Labels in assembly (`.LC0`) = a name for an address; C strings end with a hidden `\0`
- Symbols in a `.o` file (`nm`): `T` = code is here, `U` = used here, code is elsewhere
- The linker finds `U` symbols in libraries and adds startup code, which is why `hello.o` ≈ 900 B and `hello.exe` ≈ 38 KB
- Code runs before `main` (`call __main`), which on an MCU is the startup code
- Terminal: a tool that prints nothing has succeeded; `./msys` switches PowerShell into MSYS2

### What I built
- A working MSYS2 UCRT64 toolchain (GCC 16.2.0, GDB 18.1, Make 4.4.1) used from VS Code
- `exercises/00_toolchain/hello.c`, built both stage by stage and in one step with warnings
- `exercises/01_integers/q1.c`: proved `uint8_t` wrap-around and the signed/unsigned compare trap
- A Git repo pushed to a private GitHub repository

### Error zoo: results
| Bug | Change | Failed at | Key message |
|-----|--------|-----------|-------------|
| A | `#include <stdoi.h>` | 1. Preprocessor | `stdoi.h: No such file or directory` |
| B | missing `;` | 2. Compiler | `expected ';' before 'return'` |
| C | declared `prinf`, never written | 4. Linker | `undefined reference to 'prinf'` |
| D | `prinf` with no declaration | 2. Compiler | `implicit declaration of function 'prinf'` |

Cheat sheet: "No such file" → include path. "expected / implicit declaration" →
my C code. "undefined reference" → the code (.c file or library) is not in the build.

### What I can explain without help
- What each of the 4 stages does, and which stage a given error comes from
- Why `#define` names disappear before compiling

### What I need to revise
- I first thought the linker uses the `.i` file. It doesn't; it uses `.o` files and libraries.
- I first answered "missing include" for `undefined reference`. Correct: the include is fine; the **code** is missing from the build.
- Habit to build: write the prediction down **before** running.

### Open questions
- How does an MCU's startup code prepare RAM before `main`? (MCU stage)

## Milestone 1: Simulated 8-LED output port (completed 2026-10-21) ✅
### Concepts introduced
- Bit operations in real functions: set `|=`, clear `&= ~`, toggle `^=`, check `&`
- Function parameters (the caller decides the value) and return values
- Header (`.h`) = the promise, source (`.c`) = the code; include guards (`#ifndef`)
- `static` outside a function = private to that file (the port register)
- Reporting errors with a `bool` return, like a HAL's `HAL_OK` / `HAL_ERROR`
- A read function must only look, never change anything
- The off-by-one rule: with COUNT items, valid is 0..COUNT-1, so reject `>= COUNT`
- Check input where it comes in, while it is still an `int`; cast only after checking
- Signed vs unsigned compare in real code: `int pin` vs `8u` → use `(int)PORT_PIN_COUNT`
- Count-down loops (`for (int pin = 7; pin >= 0; pin--)`)
- Text input: `fgets` reads a line, `sscanf` splits it, `strcmp` returns 0 when equal
- Write a rule once: `is_valid_pin()` in C, `CFLAGS` in the Makefile
- `make`: target, ingredients, command (TAB!); rebuilds only when an ingredient is newer;
  shortcut targets `all`, `test`, `run`, `clean` with `.PHONY`
- `git diff` and `git restore` to undo unsaved changes

### What I built
- `src/port.c/.h`: the port driver (19/19 automated tests)
- `src/display.c/.h`: `PORT = 00100101   LEDs: [ . . * . . * . * ]`
- `src/main.c`: interactive command line (set, clear, toggle, read, show, help, quit)
- `Makefile`: `make`, `make run`, `make test`, `make clean`

### Bugs I made, found and fixed
| Bug | How it showed up | Lesson |
|---|---|---|
| `1 << 7` instead of `1u << pin` | always set LED 7 | use the parameter, not a fixed number |
| read used only the mask `(1u << pin)` | read 4 said true | a mask alone never looks at the port |
| read used `\|=` | reading switched the LED on | a read must not change anything |
| silent invalid pins | `set 8` did nothing, no error | return true/false and check it |
| `pin > 8` instead of `>=` (twice) | pin 8 got through | the off-by-one rule |
| `pin <= 0` | pin 0 was rejected | 0 is a valid pin |
| ignored the return of `port_set_pin` | `set 9` printed no error | always use the return value |
| `display_show(pin)`, then `display_show(0)` | show printed wrong or fixed values | use `port_get()`, the real value |
| no check in main before converting | `set 258` turned on LED 2 | 258 → uint8_t = 2 (Q1 wrap-around) |
| test rule with no ingredients | tests ran old code | `make` needs the right ingredients |

### What I can explain without help
- How set/clear/toggle/check work on a single bit
- Why a function should report errors instead of failing silently
- Why `make` says "up to date" and when it rebuilds

### What I need to revise
- Where a parameter's value comes from (the caller)
- Boundaries: `>=` vs `>`, `<` vs `<=`
- Habit: write the prediction down **before** running (still skipping it)

### Open questions
- `read` needs three answers (ON / OFF / invalid) but returns a bool. Better designs later?

## Milestone 2: Buttons (completed 2026-10-22) ✅
### Concepts introduced
- **Active low:** pull-up resistor plus a button to GND means released = 1 and pressed = 0
- **World side vs firmware side:** `press`/`release` are the finger and only move the pin;
  the firmware must discover the change by reading the pin
- **A check only reads** (`&`); and use brackets when mixing `&` with `==`,
  because `==` happens first
- **Level vs event:** "is it down?" (`input_is_pressed`) vs "did it just go down?" (`button_poll`)
- **Edge detection:** compare `before` with `now`, and remember `now` for the next poll
  (a `static` array keeps values between calls)
- **enum:** names for a set of choices (`BUTTON_EVENT_PRESSED`); they are numbers underneath
- **struct:** a blueprint for several related fields. The rule is TYPE NAME = VALUE:
  create a variable first (`Time_day x;`), then use `x.hour`
- **Array of structs = a table:** `rules[i].led` means row i, column led;
  `sizeof rules / sizeof rules[0]` = the number of rows
- **Table (what) vs engine (how):** change behaviour by editing data, not code
- **Parameters vs local variables:** `rules_apply` is GIVEN button and event;
  `rules_poll_all` FINDS them
- **Simulated time:** each command = one pass of the firmware loop; `tick N` = N passes
- Every `.c` file must be in the build, otherwise the linker reports `undefined reference`
- Windows locks a running `.exe` (`Permission denied`), so quit before rebuilding
- `git restore` to undo an experiment

### What I built
- `src/input.c/.h`: active-low input port (17 tests)
- `src/button.c/.h`: edge detection with a `button_event_t` enum (12 tests)
- `src/rules.c/.h`: rule table (`rule_t` struct) + `rules_apply` + `rules_poll_all` (13 tests)
- CLI commands `press N`, `release N`, `tick N`
- `exercises/04_struct/struct_demo.c`: struct and array-of-structs practice

### Bugs I made, found and fixed
| Bug | How it showed up | Lesson |
|---|---|---|
| `is_pressed` used `^=` | asking about button 3 flipped button 3 | a check must only read |
| `is_pressed` returned true for bit 1 | pressed/released backwards | active low: pressed = 0 |
| `input_reg & (1u << pin) == 0` | always false | `==` before `&`: use brackets |
| edge conditions called `input_press(BUTTON_COUNT)` | always NONE | use `before` and `now`, not the finger |
| `Time_day.hour = 9` | compile error | a type is a blueprint: create a variable first |
| `x[2]` holding 3 rows, loop to 3 | garbage `2:0:0` | N rows = indices 0..N-1; reading past the end |
| `rules_apply(rules[i].button, ...)` in poll | wrong event, reads past the table | pass what happened (`i`, the event) |
| `input_is_pressed` instead of `button_poll` | LED flickered every poll | level vs event (the "break it" experiment) |
| missing `input_init()` | `press 0` lit LEDs 0, 1 and 7 | initialise every module at start-up |
| simulator not linked with the new modules | `undefined reference` | the linker needs every `.c` |
| `make` while the simulator was running | `Permission denied` | quit before rebuilding |
| `tick` check `pin > 20 && pin < 1` | never true | bad = too small OR too big (`\|\|`) |

### What I can explain without help
- Why buttons read 0 when pressed
- Why edge detection is needed (I saw the flicker myself)
- Why a rule table is easier to change than a chain of `if`s

### What I need to revise
- Struct syntax: declaring a variable (TYPE NAME = VALUE) before using `.field`
- Telling apart a table's fields (`rules[i].button`) and a function's parameters (`button`)
- `&&` vs `||` in range checks
- Good: in the flicker experiment I predicted before running. Keep doing that.

### Open questions
- A press shorter than one poll is missed. (Interrupts, later.)
- Real buttons "bounce" for a few milliseconds when pressed. (Debouncing, in the timers milestone.)
