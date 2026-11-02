# Learning Log

## Current status
- Milestone: 3 complete ✅ → now: **Milestone 4, Memory (theory first)**; plan in `notes/memory/PLAN.md`
- Milestone 5 will be timers and non-blocking code (moved back from 4)
- GCC version: 16.2.0 (MSYS2 UCRT64)
- Solo 1: 11/11 ✅ · Solo 2: 18/18, zero warnings ✅ (gate passed on skill; 60 min vs the 45 limit)
- Next task: Lesson 4.1, bits, bytes and addresses

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

## Milestone 3: Device modes, a state machine (completed 2026-10-23) ✅
### Concepts introduced
- **State machine:** the same event can mean different things depending on
  what the device is doing (AC remote MODE button, microwave START button)
- **The four words:** state = where the device is now (NORMAL);
  event = what happened (button 7 PRESSED); transition = the state changes
  (`mode = MODE_TEST;`); action = what the device does (all LEDs on)
- **One enum variable, not several bools:** bools allow impossible
  combinations (cool AND heat); one variable holds exactly one state
- **switch / case / break:** `switch` asks "which state am I in?";
  forgetting `break` runs into the next case
- **Entering a state vs being in it:** entering TEST = all LEDs on (once);
  being in NORMAL = button presses go to the rule table
- **Postman vs receptionist:** `mode_poll_all` FINDS events and delivers all
  of them; `mode_handle_event` is GIVEN one event and decides
- **Only one poller:** an edge is used up by the first `button_poll` call,
  so a second poller in the same pass sees NONE
- **Read top-down, build bottom-up**, and the 5 questions before coding:
  what should happen, whose job (check the `.h`), what it gets / remembers /
  changes, the steps in words as comments, then C and test
- **An enum variable can hold any number:** `device_mode_t m = 5;` compiles
  with no warning; C never checks at run time
- **Stuck state:** a bad value matches no case, so the device ignores
  everything forever, silently
- **`default:`** catches "any value not listed"; recover to a **safe
  state**: known (mode and LEDs agree) and safe (outputs off)
- **Fault injection:** a test-only hook (`mode_inject_fault`) proves the
  recovery works; write the test first, then the fix
- **Debug output must tell the truth:** never print "OFF" for an invalid mode
- Warnings such as "unused variable" / "defined but not used" are clues of
  unfinished work

### What I built
- `exercises/05_state/ac_demo.c`: AC remote state machine (MODE and ▲ buttons)
- `src/mode.c/.h`: NORMAL → TEST → OFF state machine with safe-state
  recovery (22 tests: transitions, actions, polling, fault)
- CLI: `main.c` runs the firmware through `mode_poll_all`; new commands
  `mode` (inspect the state) and `fault N` (experiment)

### Bugs I made, found and fixed
| Bug | How it showed up | Lesson |
|---|---|---|
| trailing `\` after a function header | worked by luck | a backslash glues the next line on |
| "entering NORMAL toggles LED 0" | design table | entering a state vs being in it |
| `mode_poll_all` written in `rules.c` | would be "multiple definition" | a function lives in its own module's `.c` |
| both `rules_poll_all` and `mode_poll_all` in main | second poller would see NONE | only one poller; the first uses up the edge |
| poller passed only button 7, invented `changemode()` | other buttons would never reach the rules | the postman delivers, the receptionist decides |
| `printf` in `mode.c` | `implicit declaration` (no stdio.h) | logic modules don't print |
| function body written in `mode.h` | caught before saving | the header is only the promise |
| missing `;` after `port_set_pin(i)` | `expected ';' before '}'` | read the line and column |
| `else` without the NORMAL check | 12/19: in TEST, press 0 toggled LED 0 | TEST and OFF must ignore buttons |
| tidy-up deleted a `}` | `expected declaration or statement at end of input` | count the braces in the last edited function |
| extra `}` after `default:` | `expected identifier or '(' before 'else'` | the error points below the real mistake |
| `default:` set OFF but left the LEDs on | 21/22: mode OFF, LEDs 11111111 | a safe state needs mode AND outputs |
| main called `mode_handle_event()` | `too few arguments` | given (handle) vs finds (poll) |
| TEST printed as "ON", invalid printed as "OFF" | debug output lied | reports must use the code's names and the truth |

### What I can explain without help
- Why the same button needs to do different things in different modes
- State, event and action, with examples from my device
- Why OFF is the safe state: it is known and safe
- Why an enum variable can hold an undefined value
- My 9-step prediction for the mode cycle was 9/9 correct

### What I need to revise
- **Transition vs event:** the event is the cause (button 7 pressed); the
  transition is the state change (`mode = MODE_TEST;`)
- **Which module does what:** "held = no new event" is edge detection in `button.c`, not `mode.c`
- **Given vs finds** (`_handle_event` / `_apply` vs `_poll_all`): mixed up for the second time
- **Braces and indentation:** two brace bugs this milestone
- **Prediction habit:** skipped 3 times. When I did predict, I was right.
  Write the prediction BEFORE running.

### Optional tidy-ups (style, not bugs)
- The "all LEDs off" loop appears twice in `mode.c` (TEST case and
  `default:`); write it once as a small `static` function
- The closing `}` of the `if` in `mode_poll_all` is at the end of the file
  instead of under the `if`
- `size_t i` in `mode_poll_all` without `<stddef.h>`; `uint8_t i` matches `button_poll`

### Open questions
- `default:` only runs when button 7 is pressed; until then a bad value
  stays. Could the firmware check every poll? (Fault detection and watchdogs, later)
- If I add a new mode to the enum later, will the compiler remind me to handle it?
- TEST → OFF forgets the NORMAL LEDs. Should leaving TEST restore them?

## Solo exercise 1: pedestrian crossing light (2026-10-23, reviewed 2026-10-28)
Goal: measure what I can build alone (`exercises/06_solo/TASK.md`).
**Score: 4.5 / 10. Tests: 0/11** (it did not build). Fix by fix:
missing function → 4/11, break → 8/11, fault handling → 11/11.

### What I did alone
- Both files from a blank page; enums and all 4 prototypes correct
- The state in one `static` variable in the `.c`; init and get correct
- A switch-based state machine with my own design (switch on the event)

### Mistakes to study
| # | Mistake | Rule |
|---|---|---|
| 1 | `traffic_inject_fault` promised in `.h`, never written → linker: `undefined reference` | every prototype is a promise; tick off the `.h` against the `.c` |
| 2 | `break` only inside the `else` → GREEN + BUTTON fell through into TICK → RED | a case label is an entry point, not a wall; `break` at the end of every case |
| 3 | no fault handling; the switch was on `event`, but `light` is what gets corrupted | `default:` must be on the stored state; switch on the state first |
| 4 | header: no include guard; unused includes and `#define LED` | every header gets a guard; include only what its users need |
| 5 | the last case had no `break` | keep the habit |
| 6 | never compiled; no 5-question comments; no prediction | the compiler had already caught #1 and #2: compile early and often |

### Fix: worked example, then rebuilt alone (2026-10-28 to 2026-11-02)
- Fixing it alone was too hard, so Claude built it as a worked example in
  two compiled steps (empty `handle_event` → 3/11, then the switch → 11/11)
  and explained it one step at a time. I wrote the RED case and `default:`
  myself from the recipe.
- Then I deleted both files and rebuilt them from a blank page using my
  notes: **11/11, zero warnings, prediction right.** Time: not timed.
- The compiler caught `break` inside the `if` again
  (`this statement may fall through`). I read the warning and fixed it myself.

### What I got wrong in the walkthrough (revise these)
| Question | My answer | Right answer |
|---|---|---|
| What does `test_green` leave behind? | GREEN | YELLOW: the static remembers leftovers, and `init` wipes them |
| `traffic_get()` after `inject_fault(9)`? | RED | 9: `get` only looks; only `handle_event` changes it |
| `break` inside the `if`, with GREEN + TICK? | "stuck in a loop" | a switch is not a loop; the false `if` skips the `break`, so it falls into YELLOW → RED |
| Why switch on `light`? | unclear | `default:` only sees the value in `switch( )`, and the 9 is in `light` |
| Which header gives `uint8_t`? | `<stdio.h>` | `<stdint.h>`: the name tells you |

### Rules I'm keeping
- `break` goes outside the `if`, so it runs on every path
- In `default:`, go to the safe state with no `if`
- Write the questions in short form, one line each. Improvise while
  coding, then tick the lines off at the end.

### Gate before Milestone 4
Solo 2 (a new problem) done alone in 45 minutes or less, all tests passing,
zero warnings, with the short Q1–Q4 comments and a written prediction.

## Solo exercise 2: microwave oven (2026-11-02)
Goal: the gate before Milestone 4 (`exercises/07_solo2/TASK.md`).
**Tests: 18/18, zero warnings. Time: 60 minutes (12:15–13:15, limit 45).
Gate: 4 of 5 rules met ✅ (passed on skill, time missed).**

### What I did alone
- Both files from a blank page, with `<stdbool.h>` in the header because it uses `bool`
- `switch (state)` with a `break` outside every `if`, and `if / else if` for
  COOKING's two transitions (a pattern I hadn't used before)
- `default:` → IDLE with no `if`. The heater is on only when `state == MW_COOKING`,
  so it is safe for an unknown value
- Q1–Q4 comments written before coding. Prediction 18/18 was right, but I
  only wrote it after the review asked for it

### Mistakes to study
| # | Mistake | Rule |
|---|---|---|
| 1 | Called `microwave_heater_on();` after `state = MW_COOKING`, thinking it turns the heater on | a function that returns a value only answers a question; if the answer is thrown away, the line does nothing. The heater is on *because* the state is COOKING |
| 2 | `default:` without `break` (again: Solo 1 #5) | a `break` at the end of every case, even the last one |
| 3 | Q4 described the behaviour, not the code | Q4 is the code outline: switch → if / else if → break → default |
| 4 | 60 minutes, not 45; predictions skipped until asked | speed comes with practice; a prediction is just two numbers (passes, warnings) |

### Decision: the roadmap changed
Milestone 4 is now **Memory**: all 21 theory lessons first, then the practice
(`notes/memory/PLAN.md`). Every term gets a definition and a real example.
The goal is to be able to teach it. Timers move to Milestone 5. Solo 3 (the
45-minute retry) comes at the end of Milestone 4.
