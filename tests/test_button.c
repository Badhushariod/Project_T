/*
 * Automated tests for button events (edge detection).
 * Each check prints PASS or FAIL. The program returns 0 only if all pass.
 */
#include <stdio.h>
#include "input.h"
#include "button.h"

static int tests_run;
static int tests_failed;

/* Turn an event into readable text. */
static const char *event_name(button_event_t event)
{
    switch (event) {
    case BUTTON_EVENT_NONE:     return "NONE";
    case BUTTON_EVENT_PRESSED:  return "PRESSED";
    case BUTTON_EVENT_RELEASED: return "RELEASED";
    }
    return "???";
}

/* Poll one button and check the event it reports. */
static void check_poll(const char *what, uint8_t button, button_event_t expected)
{
    button_event_t actual = button_poll(button);

    tests_run++;
    if (actual == expected) {
        printf("PASS  %s\n", what);
        return;
    }
    tests_failed++;
    printf("FAIL  %s\n      expected %s, got %s\n", what,
           event_name(expected), event_name(actual));
}

static void start(void)
{
    input_init();
    button_init();
}

static void test_nothing_pressed(void)
{
    start();
    check_poll("nothing pressed: NONE", 0, BUTTON_EVENT_NONE);
}

static void test_press_hold_release(void)
{
    start();
    input_press(2);
    check_poll("press 2: PRESSED", 2, BUTTON_EVENT_PRESSED);
    check_poll("still held: NONE", 2, BUTTON_EVENT_NONE);
    check_poll("still held again: NONE", 2, BUTTON_EVENT_NONE);
    input_release(2);
    check_poll("release 2: RELEASED", 2, BUTTON_EVENT_RELEASED);
    check_poll("still released: NONE", 2, BUTTON_EVENT_NONE);
}

static void test_press_again(void)
{
    start();
    input_press(6);
    button_poll(6);
    input_release(6);
    button_poll(6);
    input_press(6);
    check_poll("second press of 6: PRESSED again", 6, BUTTON_EVENT_PRESSED);
}

static void test_buttons_are_independent(void)
{
    start();
    input_press(1);
    input_press(4);
    check_poll("button 1: PRESSED", 1, BUTTON_EVENT_PRESSED);
    check_poll("button 4: PRESSED", 4, BUTTON_EVENT_PRESSED);
    check_poll("button 1 again: NONE", 1, BUTTON_EVENT_NONE);
    check_poll("button 0 untouched: NONE", 0, BUTTON_EVENT_NONE);
}

static void test_invalid_button(void)
{
    start();
    check_poll("button 8: NONE", 8, BUTTON_EVENT_NONE);
}

int main(void)
{
    test_nothing_pressed();
    test_press_hold_release();
    test_press_again();
    test_buttons_are_independent();
    test_invalid_button();

    printf("\nResult: %d/%d passed\n", tests_run - tests_failed, tests_run);
    return (tests_failed == 0) ? 0 : 1;
}
