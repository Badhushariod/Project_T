/*
 * Automated tests for the port module.
 * Each check prints PASS or FAIL. The program returns 0 only if all pass.
 */
#include <stdio.h>
#include "port.h"

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

/* Check that the whole port has the expected value. */
static void check_port(const char *what, uint8_t expected)
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

/* Check that port_read_pin() gives the expected answer. */
static void check_pin(const char *what, uint8_t pin, bool expected)
{
    bool actual = port_read_pin(pin);

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
    port_init();
    check_port("init: all LEDs off", 0x00);                 /* 0000 0000 */
}

static void test_set(void)
{
    port_init();
    port_set_pin(0);
    check_port("set pin 0", 0x01);                          /* 0000 0001 */
    port_set_pin(7);
    check_port("set pin 7, pin 0 stays on", 0x81);          /* 1000 0001 */
    port_set_pin(7);
    check_port("set pin 7 again: no change", 0x81);         /* 1000 0001 */
}

static void test_clear(void)
{
    port_init();
    port_set_pin(0);
    port_set_pin(7);
    port_clear_pin(0);
    check_port("clear pin 0, pin 7 stays on", 0x80);        /* 1000 0000 */
    port_clear_pin(0);
    check_port("clear pin 0 again: no change", 0x80);       /* 1000 0000 */
}

static void test_toggle(void)
{
    port_init();
    port_toggle_pin(3);
    check_port("toggle pin 3: off -> on", 0x08);            /* 0000 1000 */
    port_toggle_pin(3);
    check_port("toggle pin 3 again: on -> off", 0x00);      /* 0000 0000 */
}

static void test_read(void)
{
    port_init();
    port_set_pin(5);
    check_pin("read pin 5 after set: true", 5, true);
    check_pin("read pin 4 (never set): false", 4, false);
}

int main(void)
{
    test_init();
    test_set();
    test_clear();
    test_toggle();
    test_read();

    printf("\nResult: %d/%d passed\n", tests_run - tests_failed, tests_run);
    return (tests_failed == 0) ? 0 : 1;
}
