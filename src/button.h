#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "input.h"

/*
 * Button events (firmware side).
 *
 * The input port tells us the LEVEL of a button: pressed or not, right now.
 * Usually firmware needs the EVENT: did it just change?
 *
 *   before   now      event
 *   ------   ------   ---------------------
 *   up       up       BUTTON_EVENT_NONE
 *   up       down     BUTTON_EVENT_PRESSED
 *   down     down     BUTTON_EVENT_NONE      <- held: no new event
 *   down     up       BUTTON_EVENT_RELEASED
 */

#define BUTTON_COUNT INPUT_PIN_COUNT

/* An enum gives names to a set of choices. */
typedef enum {
    BUTTON_EVENT_NONE,       /* nothing changed since the last poll */
    BUTTON_EVENT_PRESSED,    /* was up, now down                     */
    BUTTON_EVENT_RELEASED    /* was down, now up                     */
} button_event_t;

void           button_init(void);              /* forget history: all buttons up   */
button_event_t button_poll(uint8_t button);    /* check one button, return its event;
                                                  NONE for an invalid button        */

#endif /* BUTTON_H */
