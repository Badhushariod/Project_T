# Learning Log

## Current status
- Milestone: 0 complete ✅ → next: Milestone 1 (simulated 8-LED output port)
- GCC version: 16.2.0 (MSYS2 UCRT64)
- Next task: Milestone 1, Lesson 1: bit operations on a simulated port register

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
