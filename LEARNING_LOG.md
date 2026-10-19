# Learning Log

## Current status
- Milestone: 0 (Toolchain ready)
- GCC version: 16.2.0 (MSYS2 UCRT64)
- Next task: Milestone 0, Part D: build hello.c in four stages, then the error zoo

## Starting assessment (2026-10-19)
Background: Nuvoton, ESP, Keil. I know ISRs, direct register access and
scatter files as ideas, but I haven't written them myself yet.

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

## Milestone 0: Toolchain ready (date: )
### Concepts introduced
-
### What I built
-
### Error zoo: predictions vs results
| Bug | My prediction (which stage fails + rough message) | What actually happened |
|-----|---------------------------------------------------|------------------------|
| A   |                                                   |                        |
| B   |                                                   |                        |
| C   |                                                   |                        |
| D   |                                                   |                        |
### What I can explain without help
-
### What I need to revise
-
### Open questions
-
