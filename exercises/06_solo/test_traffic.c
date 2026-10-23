/*
 * Automated tests for solo exercise 1: the pedestrian crossing light.
 * See TASK.md for the behaviour table. Each check prints PASS or FAIL.
 * The program returns 0 only if all checks pass.
 */
#include <stdio.h>
#include "traffic.h"

static int tests_run;
static int tests_failed;

/* A printable name for a light, only for the test messages. */
static const char *name_of(light_t light)
{
    static const char *names[] = { "GREEN", "YELLOW", "RED" };
    if ((int)light < 0 || (int)light > 2) {
        return "INVALID";
    }
    return names[light];
}

static void check(const char *what, light_t expected)
{
    light_t actual = traffic_get();

    tests_run++;
    if (actual == expected) {
        printf("PASS  %s\n", what);
        return;
    }
    tests_failed++;
    printf("FAIL  %s\n      expected %s, got %s\n",
           what, name_of(expected), name_of(actual));
}

/* ---- Normal behaviour: every row and column of the table ---- */

static void test_start(void)
{
    traffic_init();
    check("starts GREEN", LIGHT_GREEN);
}

static void test_green(void)
{
    traffic_init();
    traffic_handle_event(EVENT_TICK);
    check("GREEN + TICK -> GREEN (waits for a pedestrian)", LIGHT_GREEN);
    traffic_handle_event(EVENT_BUTTON);
    check("GREEN + BUTTON -> YELLOW", LIGHT_YELLOW);
}

static void test_yellow(void)
{
    traffic_init();
    traffic_handle_event(EVENT_BUTTON);             /* now YELLOW */
    traffic_handle_event(EVENT_BUTTON);
    check("YELLOW + BUTTON -> YELLOW (ignored)", LIGHT_YELLOW);
    traffic_handle_event(EVENT_TICK);
    check("YELLOW + TICK -> RED", LIGHT_RED);
}

static void test_red(void)
{
    traffic_init();
    traffic_handle_event(EVENT_BUTTON);             /* now YELLOW */
    traffic_handle_event(EVENT_TICK);               /* now RED    */
    traffic_handle_event(EVENT_BUTTON);
    check("RED + BUTTON -> RED (ignored)", LIGHT_RED);
    traffic_handle_event(EVENT_TICK);
    check("RED + TICK -> GREEN", LIGHT_GREEN);
}

static void test_two_crossings(void)
{
    traffic_init();
    traffic_handle_event(EVENT_BUTTON);
    traffic_handle_event(EVENT_TICK);
    traffic_handle_event(EVENT_TICK);
    traffic_handle_event(EVENT_BUTTON);
    check("second pedestrian: GREEN -> YELLOW again", LIGHT_YELLOW);
}

/* ---- Fault: an unknown state value must go to the safe state ---- */

static void test_fault(void)
{
    traffic_init();
    traffic_inject_fault(9);
    traffic_handle_event(EVENT_TICK);
    check("fault 9 + TICK -> RED (safe state)", LIGHT_RED);

    traffic_init();
    traffic_inject_fault(9);
    traffic_handle_event(EVENT_BUTTON);
    check("fault 9 + BUTTON -> RED (safe state)", LIGHT_RED);

    traffic_handle_event(EVENT_TICK);
    check("after recovery: RED + TICK -> GREEN", LIGHT_GREEN);
}

int main(void)
{
    test_start();
    test_green();
    test_yellow();
    test_red();
    test_two_crossings();
    test_fault();

    printf("\nResult: %d/%d passed\n", tests_run - tests_failed, tests_run);
    return (tests_failed == 0) ? 0 : 1;
}
