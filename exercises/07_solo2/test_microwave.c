/*
 * Automated tests for solo exercise 2: the microwave oven.
 * See TASK.md for the behaviour table. Each check prints PASS or FAIL.
 * The program returns 0 only if all checks pass.
 */
#include <stdio.h>
#include "microwave.h"

static int tests_run;
static int tests_failed;

/* A printable name for a state, only for the test messages. */
static const char *name_of(mw_state_t state)
{
    static const char *names[] = { "IDLE", "COOKING", "PAUSED" };
    if ((int)state < 0 || (int)state > 2) {
        return "INVALID";
    }
    return names[state];
}

static void check_state(const char *what, mw_state_t expected)
{
    mw_state_t actual = microwave_get();

    tests_run++;
    if (actual == expected) {
        printf("PASS  %s\n", what);
        return;
    }
    tests_failed++;
    printf("FAIL  %s\n      expected %s, got %s\n",
           what, name_of(expected), name_of(actual));
}

static void check_heater(const char *what, bool expected)
{
    bool actual = microwave_heater_on();

    tests_run++;
    if (actual == expected) {
        printf("PASS  %s\n", what);
        return;
    }
    tests_failed++;
    printf("FAIL  %s\n      expected heater %s, got %s\n",
           what, expected ? "ON" : "OFF", actual ? "ON" : "OFF");
}

/* ---- Normal behaviour: every row and column of the table ---- */

static void test_start(void)
{
    microwave_init();
    check_state("starts IDLE", MW_IDLE);
    check_heater("IDLE: heater off", false);
}

static void test_idle(void)
{
    microwave_init();
    microwave_handle_event(MW_EVENT_DOOR_OPEN);
    check_state("IDLE + DOOR_OPEN -> IDLE (ignored)", MW_IDLE);
    microwave_handle_event(MW_EVENT_TIMER_DONE);
    check_state("IDLE + TIMER_DONE -> IDLE (ignored)", MW_IDLE);
    microwave_handle_event(MW_EVENT_START);
    check_state("IDLE + START -> COOKING", MW_COOKING);
}

static void test_cooking(void)
{
    microwave_init();
    microwave_handle_event(MW_EVENT_START);         /* now COOKING */
    check_heater("COOKING: heater on", true);
    microwave_handle_event(MW_EVENT_START);
    check_state("COOKING + START -> COOKING (ignored)", MW_COOKING);
    microwave_handle_event(MW_EVENT_DOOR_OPEN);
    check_state("COOKING + DOOR_OPEN -> PAUSED", MW_PAUSED);
}

static void test_paused(void)
{
    microwave_init();
    microwave_handle_event(MW_EVENT_START);         /* now COOKING */
    microwave_handle_event(MW_EVENT_DOOR_OPEN);     /* now PAUSED  */
    check_heater("PAUSED: heater off", false);
    microwave_handle_event(MW_EVENT_DOOR_OPEN);
    microwave_handle_event(MW_EVENT_TIMER_DONE);
    check_state("PAUSED + DOOR_OPEN, TIMER_DONE -> PAUSED (ignored)", MW_PAUSED);
    microwave_handle_event(MW_EVENT_START);
    check_state("PAUSED + START -> COOKING (carries on)", MW_COOKING);
}

static void test_done(void)
{
    microwave_init();
    microwave_handle_event(MW_EVENT_START);         /* now COOKING */
    microwave_handle_event(MW_EVENT_TIMER_DONE);
    check_state("COOKING + TIMER_DONE -> IDLE", MW_IDLE);
    check_heater("done: heater off", false);
}

/* ---- Fault: an unknown state value must go to the safe state ---- */

static void test_fault(void)
{
    microwave_init();
    microwave_inject_fault(9);
    check_heater("fault 9: heater off (before any event)", false);
    microwave_handle_event(MW_EVENT_START);
    check_state("fault 9 + START -> IDLE (safe state)", MW_IDLE);

    microwave_init();
    microwave_inject_fault(9);
    microwave_handle_event(MW_EVENT_DOOR_OPEN);
    check_state("fault 9 + DOOR_OPEN -> IDLE (safe state)", MW_IDLE);

    microwave_init();
    microwave_inject_fault(9);
    microwave_handle_event(MW_EVENT_TIMER_DONE);
    check_state("fault 9 + TIMER_DONE -> IDLE (safe state)", MW_IDLE);

    microwave_handle_event(MW_EVENT_START);
    check_state("after recovery: IDLE + START -> COOKING", MW_COOKING);
}

int main(void)
{
    test_start();
    test_idle();
    test_cooking();
    test_paused();
    test_done();
    test_fault();

    printf("\nResult: %d/%d passed\n", tests_run - tests_failed, tests_run);
    return (tests_failed == 0) ? 0 : 1;
}
