# Solo exercise 1: pedestrian crossing light

**Goal:** find out what you can build **alone**. No hints, and no AI.
You *may* use your own notes, `LEARNING_LOG.md` and your old code
(`mode.c`, `ac_demo.c`). Real engineers use those too.

**Timebox:** 45 minutes. Write down your start time. If you are not done
after 60 minutes, stop and send what you have. That is useful information,
not a failure.

## The device
A traffic light at a pedestrian crossing. It stays GREEN for the cars until
a pedestrian presses the button. Then it goes YELLOW, then RED (the
pedestrians cross), then GREEN again. Time moves in ticks.

## Behaviour table
| Now               | event TICK | event BUTTON      |
|-------------------|------------|-------------------|
| GREEN (start)     | GREEN      | YELLOW            |
| YELLOW            | RED        | YELLOW (ignored)  |
| RED               | GREEN      | RED (ignored)     |
| any unknown value | RED        | RED               |

Why RED for an unknown value: on a road, the safe state is the one where
the cars are stopped.

## What you write
Two **new** files in this folder: `traffic.h` and `traffic.c`.

`traffic.h` must provide exactly these names, because the test uses them:
```c
typedef enum { LIGHT_GREEN, LIGHT_YELLOW, LIGHT_RED } light_t;
typedef enum { EVENT_TICK, EVENT_BUTTON } traffic_event_t;

void    traffic_init(void);                          /* start in GREEN            */
light_t traffic_get(void);                           /* the current light         */
void    traffic_handle_event(traffic_event_t event); /* the state machine         */
void    traffic_inject_fault(int raw_value);         /* TEST ONLY: corrupt state  */
```
Everything else is your decision: where the state is stored, how the code
is structured, and the comments.

## The 5 questions (ask them before writing any code)
| # | Question | Where to look |
|---|---|---|
| 1 | What should happen? | the behaviour table above |
| 2 | Whose job is it? | which file, which function does each part belong in? |
| 3 | What does it get, remember, change? | parameters (gets), `static` variables (remembers), what it modifies (changes) |
| 4 | Write the steps in words | as comments inside the function |
| 5 | Translate to C, then test | one small step at a time; compile, run, read the output |

## Rules
1. **Before coding:** answer questions 1-4 as comments in `traffic.c`.
2. **Before running the tests:** write down how many of the 11 checks you
   expect to pass.
3. **Stuck for more than 10 minutes on one thing?** Write down what you are
   stuck on, then work on another part. Don't ask me.
4. **When you are done:** send me your prediction, the test output, how long
   it took, and where you got stuck.

## Build and test (from the project root, in the MSYS2 shell)
```
mkdir -p build
gcc -std=c11 -Wall -Wextra -Wpedantic exercises/06_solo/traffic.c exercises/06_solo/test_traffic.c -o build/test_traffic.exe
./build/test_traffic.exe
```
**Done when:** 11/11 pass with zero warnings.
