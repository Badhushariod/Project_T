/*
 * Automated tests for the device-mode state machine.
 * Each check prints PASS or FAIL. The program returns 0 only if all pass.
 *
 *             press 7          press 7          press 7
 *   NORMAL ──────────► TEST ──────────► OFF ──────────► NORMAL ...
 *
 *   Part A: transitions (does the mode change correctly?)
 *   Part B: actions     (do the LEDs do the right thing in each mode?)
 *   Part C: polling     (real button presses through button_poll)
 */
#include <stdio.h>
#include "port.h"
#include "input.h"
#include "button.h"
#include "mode.h"

static int tests_run;
static int tests_failed;

/* A printable name for a mode, only for the test messages. */
static const char *name_of(device_mode_t m)
{
    static const char *names[] = { "OFF", "NORMAL", "TEST" };
    if ((int)m < 0 || (int)m > 2) {
        return "INVALID";
    }
    return names[m];
}

/* Turn a value into 8 characters of '0'/'1', bit 7 first. */
static void to_bits(uint8_t value, char out[9])
{
    for (int i = 0; i < 8; i++) {
        out[i] = (value & (1u << (7 - i))) ? '1' : '0';
    }
    out[8] = '\0';
}

/* Check that the device is in the expected mode. */
static void check_mode(const char *what, device_mode_t expected)
{
    device_mode_t actual = mode_get();

    tests_run++;
    if (actual == expected) {
        printf("PASS  %s\n", what);
        return;
    }
    tests_failed++;
    printf("FAIL  %s\n      expected %s, got %s\n",
           what, name_of(expected), name_of(actual));
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
    mode_init();
}

/* One press of the MODE button, given straight to the state machine. */
static void press_mode(void)
{
    mode_handle_event(MODE_BUTTON, BUTTON_EVENT_PRESSED);
}

/* ---- Part A: transitions ---- */

static void test_start_mode(void)
{
    start();
    check_mode("A: the device starts in NORMAL", MODE_NORMAL);
}

static void test_cycle(void)
{
    start();
    press_mode();
    check_mode("A: NORMAL + press 7 -> TEST", MODE_TEST);
    press_mode();
    check_mode("A: TEST + press 7 -> OFF", MODE_OFF);
    press_mode();
    check_mode("A: OFF + press 7 -> NORMAL", MODE_NORMAL);
}

static void test_no_transition(void)
{
    start();
    mode_handle_event(MODE_BUTTON, BUTTON_EVENT_RELEASED);
    check_mode("A: release 7 -> no change", MODE_NORMAL);
    mode_handle_event(0, BUTTON_EVENT_PRESSED);
    check_mode("A: NORMAL + press 0 -> still NORMAL", MODE_NORMAL);
    press_mode();
    mode_handle_event(0, BUTTON_EVENT_PRESSED);
    check_mode("A: TEST + press 0 -> still TEST", MODE_TEST);
}

/* ---- Part B: actions ---- */

static void test_enter_test(void)
{
    start();
    press_mode();
    check_leds("B: enter TEST -> all LEDs on", 0xFF);         /* 1111 1111 */
}

static void test_enter_off(void)
{
    start();
    press_mode();
    press_mode();
    check_leds("B: enter OFF -> all LEDs off", 0x00);         /* 0000 0000 */
}

static void test_normal_uses_rules(void)
{
    start();
    mode_handle_event(0, BUTTON_EVENT_PRESSED);
    check_leds("B: NORMAL + press 0 -> LED 0 on (rule table)", 0x01);
}

static void test_test_ignores_buttons(void)
{
    start();
    press_mode();
    mode_handle_event(0, BUTTON_EVENT_PRESSED);
    check_leds("B: TEST + press 0 -> all LEDs stay on", 0xFF);
    mode_handle_event(2, BUTTON_EVENT_PRESSED);
    check_leds("B: TEST + press 2 -> LED 7 stays on", 0xFF);
}

static void test_off_ignores_buttons(void)
{
    start();
    press_mode();
    press_mode();
    mode_handle_event(0, BUTTON_EVENT_PRESSED);
    check_leds("B: OFF + press 0 -> all LEDs stay off", 0x00);
}

static void test_back_to_normal(void)
{
    start();
    mode_handle_event(0, BUTTON_EVENT_PRESSED);     /* NORMAL: LED 0 on  */
    press_mode();                                   /* TEST:   all on    */
    press_mode();                                   /* OFF:    all off   */
    press_mode();                                   /* NORMAL again      */
    check_leds("B: OFF -> NORMAL: LEDs stay dark", 0x00);
    mode_handle_event(0, BUTTON_EVENT_PRESSED);
    check_leds("B: back in NORMAL: press 0 -> LED 0 on", 0x01);
}

/* ---- Part C: polling with real button presses ---- */

static void test_poll_mode_button(void)
{
    start();
    input_press(MODE_BUTTON);
    mode_poll_all();
    check_mode("C: poll: press 7 -> TEST", MODE_TEST);
    mode_poll_all();
    mode_poll_all();
    check_mode("C: poll: 7 still held -> still TEST (one press = one step)", MODE_TEST);
    input_release(MODE_BUTTON);
    mode_poll_all();
    check_mode("C: poll: release 7 -> still TEST", MODE_TEST);
}

static void test_poll_rules_in_normal(void)
{
    start();
    input_press(0);
    mode_poll_all();
    check_leds("C: poll in NORMAL: press 0 -> LED 0 on", 0x01);
}

int main(void)
{
    test_start_mode();
    test_cycle();
    test_no_transition();

    test_enter_test();
    test_enter_off();
    test_normal_uses_rules();
    test_test_ignores_buttons();
    test_off_ignores_buttons();
    test_back_to_normal();

    test_poll_mode_button();
    test_poll_rules_in_normal();

    printf("\nResult: %d/%d passed\n", tests_run - tests_failed, tests_run);
    return (tests_failed == 0) ? 0 : 1;
}
