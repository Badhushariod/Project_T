#ifndef MODE_H
#define MODE_H

#include <stdint.h>
#include "button.h"

/*
 * Device modes: a state machine.
 *
 *             press 7          press 7          press 7
 *   NORMAL ──────────► TEST ──────────► OFF ──────────► NORMAL ...
 *
 *   mode     LEDs                         buttons 0-6
 *   ------   --------------------------   ---------------------------
 *   NORMAL   driven by the rule table     events go to rules_apply()
 *   TEST     all on (lamp test)           ignored
 *   OFF      all off                      ignored
 *
 * The device starts in NORMAL, so it behaves like Milestone 2 until
 * button 7 is pressed.
 *
 * Every button event now goes to the state machine FIRST:
 *
 *   button_poll -> event -> mode_handle_event -> (NORMAL only) rules_apply -> port
 */

#define MODE_BUTTON 7u          /* the button that changes the mode */

/* The states. */
typedef enum {
    MODE_OFF,
    MODE_NORMAL,
    MODE_TEST
} device_mode_t;

void          mode_init(void);  /* start in NORMAL                   */
device_mode_t mode_get(void);   /* the current mode (only looks)     */

/*
 * The state machine: give it ONE button event.
 * Depending on the current mode it may change the mode (transition)
 * and/or change LEDs (action).
 */
void mode_handle_event(uint8_t button, button_event_t event);

/*
 * Poll every button once and pass each event to mode_handle_event().
 * In the firmware loop this replaces rules_poll_all().
 */
void mode_poll_all(void);

/*
 * FOR TESTS AND EXPERIMENTS ONLY: overwrite the state variable with any raw
 * value, the way a memory bug would (e.g. 5, which is not a mode).
 * Real firmware would not ship with this.
 */
void mode_inject_fault(int raw_value);

#endif /* MODE_H */
