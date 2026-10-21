/*
 * Automated tests for the button -> LED rules.
 * Each check prints PASS or FAIL. The program returns 0 only if all pass.
 *
 * Rules under test (see src/rules.c):
 *   button 0 PRESSED  -> toggle LED 0
 *   button 1 PRESSED  -> LED 1 on
 *   button 1 RELEASED -> LED 1 off
 *   button 2 PRESSED  -> toggle LED 7
 */
#include <stdio.h>
#include "port.h"
#include "input.h"
#include "button.h"
#include "rules.h"

static int tests_run;
static int tests_failed;

/* Turn a value into 8 characters of '0'/'1', bit 7 first. */
static void to_bits(uint8_t value, char out[9])
{
    for (int i = 0; i < 8; i++) {
        out[i] = (value & (1u << (7 - i))) ? '1' : '0';
    }
    out[8] = '\0';
}

/* Check that the LED port has the expected value. */
static void check_leds(const char *what, uint8_t expected)
{
    uint8_t actual = port_get();
    char exp_bits[9];
    char act_bits[9];

    tests_run++;
    if (actual == expected) {
        printf("PASS  %s\n", what);
        return;
    }
    tests_failed++;
    to_bits(expected, exp_bits);
    to_bits(actual, act_bits);
    printf("FAIL  %s\n      expected %s, got %s\n", what, exp_bits, act_bits);
}

static void start(void)
{
    port_init();
    input_init();
    button_init();
}

/* ---- Part 1: rules_apply() on its own ---- */

static void test_apply_toggle(void)
{
    start();
    rules_apply(0, BUTTON_EVENT_PRESSED);
    check_leds("button 0 pressed: LED 0 on", 0x01);            /* 0000 0001 */
    rules_apply(0, BUTTON_EVENT_PRESSED);
    check_leds("button 0 pressed again: LED 0 off", 0x00);     /* 0000 0000 */
    rules_apply(0, BUTTON_EVENT_RELEASED);
    check_leds("button 0 released: no rule, no change", 0x00);
}

static void test_apply_on_off(void)
{
    start();
    rules_apply(1, BUTTON_EVENT_PRESSED);
    check_leds("button 1 pressed: LED 1 on", 0x02);            /* 0000 0010 */
    rules_apply(1, BUTTON_EVENT_RELEASED);
    check_leds("button 1 released: LED 1 off", 0x00);
}

static void test_apply_other_led(void)
{
    start();
    rules_apply(2, BUTTON_EVENT_PRESSED);
    check_leds("button 2 pressed: LED 7 on", 0x80);            /* 1000 0000 */
    rules_apply(5, BUTTON_EVENT_PRESSED);
    check_leds("button 5 pressed: no rule, no change", 0x80);
}

/* ---- Part 2: rules_poll_all() with real button presses ---- */

static void test_poll_toggle_no_flicker(void)
{
    start();
    input_press(0);
    rules_poll_all();
    check_leds("poll: press 0 -> LED 0 on", 0x01);
    rules_poll_all();
    rules_poll_all();
    check_leds("poll: 0 still held -> LED 0 stays on (no flicker)", 0x01);
    input_release(0);
    rules_poll_all();
    check_leds("poll: release 0 -> LED 0 stays on", 0x01);
    input_press(0);
    rules_poll_all();
    check_leds("poll: press 0 again -> LED 0 off", 0x00);
}

static void test_poll_follow_button(void)
{
    start();
    input_press(1);
    rules_poll_all();
    check_leds("poll: hold 1 -> LED 1 on", 0x02);
    input_release(1);
    rules_poll_all();
    check_leds("poll: release 1 -> LED 1 off", 0x00);
}

int main(void)
{
    test_apply_toggle();
    test_apply_on_off();
    test_apply_other_led();
    test_poll_toggle_no_flicker();
    test_poll_follow_button();

    printf("\nResult: %d/%d passed\n", tests_run - tests_failed, tests_run);
    return (tests_failed == 0) ? 0 : 1;
}
