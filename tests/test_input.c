/*
 * Automated tests for the input (buttons) module.
 * Each check prints PASS or FAIL. The program returns 0 only if all pass.
 */
#include <stdio.h>
#include "input.h"

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

/* Check that the whole input register has the expected value. */
static void check_reg(const char *what, uint8_t expected)
{
    uint8_t actual = input_get();
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

/* Check that a function returned the expected true/false. */
static void check_result(const char *what, bool actual, bool expected)
{
    tests_run++;
    if (actual == expected) {
        printf("PASS  %s\n", what);
        return;
    }
    tests_failed++;
    printf("FAIL  %s\n      expected %s, got %s\n", what,
           expected ? "true" : "false", actual ? "true" : "false");
}

static void test_init(void)
{
    input_init();
    check_reg("init: no button pressed, all pins 1", 0xFF);    /* 1111 1111 */
    check_result("init: button 0 not pressed", input_is_pressed(0), false);
}

static void test_press(void)
{
    input_init();
    check_result("press 2 returns true", input_press(2), true);
    check_reg("press 2: pin 2 becomes 0", 0xFB);                /* 1111 1011 */
    check_result("button 2 is pressed", input_is_pressed(2), true);
    check_result("button 3 is not pressed", input_is_pressed(3), false);
    input_press(2);
    check_reg("press 2 again: no change", 0xFB);               /* 1111 1011 */
    input_press(5);
    check_reg("press 5: pin 2 stays pressed", 0xDB);           /* 1101 1011 */
}

static void test_release(void)
{
    input_init();
    input_press(2);
    input_press(5);
    check_result("release 2 returns true", input_release(2), true);
    check_reg("release 2: pin 2 back to 1, pin 5 stays 0", 0xDF); /* 1101 1111 */
    check_result("button 2 no longer pressed", input_is_pressed(2), false);
    input_release(2);
    check_reg("release 2 again: no change", 0xDF);             /* 1101 1111 */
}

static void test_invalid_pins(void)
{
    input_init();
    input_press(1);                                             /* 1111 1101 */
    check_result("press 8 returns false", input_press(8), false);
    check_result("release 8 returns false", input_release(8), false);
    check_result("press 40 returns false", input_press(40), false);
    check_reg("bad pins: register unchanged", 0xFD);           /* 1111 1101 */
    check_result("is_pressed 8: false", input_is_pressed(8), false);
}

int main(void)
{
    test_init();
    test_press();
    test_release();
    test_invalid_pins();

    printf("\nResult: %d/%d passed\n", tests_run - tests_failed, tests_run);
    return (tests_failed == 0) ? 0 : 1;
}
