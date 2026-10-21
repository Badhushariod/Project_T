# Learning Log

## Current status
- Milestone: 1 complete ✅ → next: Milestone 2 (to be agreed)
- GCC version: 16.2.0 (MSYS2 UCRT64)
- Next task: start Milestone 2

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
