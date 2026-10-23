/*
 * Lesson 3.1 demo: a tiny state machine for an AC remote's MODE button.
 *
 *          MODE          MODE
 *   COOL ───────► HEAT ───────► FAN
 *    ▲                           │
 *    └────────── MODE ───────────┘
 *
 * Build and run (from the project root, in the MSYS2 shell):
 *   mkdir -p build
 *   gcc -std=c11 -Wall -Wextra -Wpedantic exercises/05_state/ac_demo.c -o build/ac_demo.exe
 *   ./build/ac_demo.exe
 */
#include <stdio.h>

/* The states: one name per mode. */
typedef enum {
    AC_COOL,
    AC_HEAT,
    AC_FAN
} ac_mode_t;

/*
 * The state variable. ONE variable, so the AC is always in exactly one mode.
 * 'static': private to this file, and the value is kept between calls
 * (like was_pressed[] in button.c).
 */
static ac_mode_t mode = AC_COOL;

/* Event: the MODE button was pressed. */
static void on_mode_button(void)
{
    switch (mode) {                 /* "which state am I in?"         */
    case AC_COOL:
        mode = AC_HEAT;             /* transition: COOL -> HEAT       */
        printf("heater on\n");      /* action                         */
        break;                      /* done: leave the switch         */
    case AC_HEAT:
        mode = AC_FAN;              /* transition: HEAT -> FAN        */
        printf("fan only\n");
        break;
    case AC_FAN:
        mode = AC_COOL;             /* transition: FAN -> COOL        */
        printf("cooling on\n");
        break;
    }
}
static void on_up_button(void)
{
    switch (mode) {                 /* "which state am I in?"         */
    case AC_COOL:
        printf("temp+1\n");
        break;
    case AC_HEAT:
        printf("temp+1\n");
        break;
    case AC_FAN:
        printf("fan +1\n");
        break;
    }
}

int main(void)
{
    on_mode_button();
    on_up_button();
    on_mode_button();
    on_up_button();
    on_mode_button();
    on_up_button();
    on_up_button();
    on_mode_button();
    on_up_button();
    on_up_button();
    return 0;
}
