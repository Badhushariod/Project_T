# Solo exercise 2: microwave oven

**Goal:** pass the gate before Milestone 4. Build it **alone**. No hints, and no AI.
You *may* use your own notes, `LEARNING_LOG.md` and your old code
(`mode.c`, `exercises/06_solo/traffic.c`). Real engineers use those too.

**Timebox:** 45 minutes. **Write down your start time before you read on.**
If you are not done after 60 minutes, stop and send what you have. That is
useful information, not a failure.

## The device
A microwave oven. You press START and it cooks. If you open the door while it
cooks, it pauses and the heater goes off. Press START again and it carries on
cooking. (To keep it simple: pressing START means you have closed the door
again.) When the timer runs out, it is done and goes back to IDLE.

## Behaviour table
| Now               | event START       | event DOOR_OPEN   | event TIMER_DONE  |
|-------------------|-------------------|-------------------|-------------------|
| IDLE (start)      | COOKING           | IDLE (ignored)    | IDLE (ignored)    |
| COOKING           | COOKING (ignored) | PAUSED            | IDLE              |
| PAUSED            | COOKING           | PAUSED (ignored)  | PAUSED (ignored)  |
| any unknown value | IDLE              | IDLE              | IDLE              |

**The heater** is on only in COOKING. In every other state, **including an
unknown value**, it is off.

Why IDLE for an unknown value: a heater that runs while nobody knows the
state is a fire risk. The safe state is the one where the heater is off.

## What you write
Two **new** files in this folder: `microwave.h` and `microwave.c`.

`microwave.h` must provide exactly these names, because the test uses them:
```c
typedef enum { MW_IDLE, MW_COOKING, MW_PAUSED } mw_state_t;
typedef enum { MW_EVENT_START, MW_EVENT_DOOR_OPEN, MW_EVENT_TIMER_DONE } mw_event_t;

void       microwave_init(void);                     /* start in IDLE             */
mw_state_t microwave_get(void);                      /* the current state         */
void       microwave_handle_event(mw_event_t event); /* the state machine         */
bool       microwave_heater_on(void);                /* true only while COOKING   */
void       microwave_inject_fault(int raw_value);    /* TEST ONLY: corrupt state  */
```
Everything else is your decision: where the state is stored, how the code
is structured, and the comments.

## The questions (short form, one line each)
| # | Question | Write |
|---|---|---|
| 1 | What should happen? | the table, one line per box |
| 2 | Whose job is it? | which file, which function |
| 3 | What does it get, remember, change? | gets / remembers / changes |
| 4 | Steps in words | the outline of the code, in one line |
| 5 | Translate to C, then test | one small step at a time; compile, run, read the output |

## Rules
1. **Before coding:** write Q1-Q4 in short form as a comment at the top of
   `microwave.c`.
2. **Before every run:** write down how many of the 18 checks you expect to pass.
3. **Stuck for more than 10 minutes on one thing?** Write down what you are
   stuck on, then work on another part. Don't ask me.
4. **When you are done:** tick your Q1-Q4 lines off against your code. Then
   send me:
   - your start time and end time
   - every prediction you wrote, and the output of that run
   - the final output, **including the gcc line** (so the warnings show)
   - where you got stuck, and what you looked up in your notes

## Build and test (from the project root, in the MSYS2 shell)
```
mkdir -p build
gcc -std=c11 -Wall -Wextra -Wpedantic exercises/07_solo2/microwave.c exercises/07_solo2/test_microwave.c -o build/test_microwave.exe
./build/test_microwave.exe
```
**Done when:** 18/18 pass with zero warnings, within 45 minutes, with the
Q1-Q4 comments and a written prediction.
