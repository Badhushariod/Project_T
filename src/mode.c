#include "mode.h"
#include "port.h"
#include "rules.h"

/*
 * The state variable: the ONE place where the current mode is stored.
 * 'static': private to this file, and kept between calls.
 */
static device_mode_t mode;

void mode_init(void)
{
    mode = MODE_NORMAL;
}

device_mode_t mode_get(void)
{
    return mode;
}

void mode_handle_event(uint8_t button, button_event_t event)
{
    /* TODO step A - transitions:
     *     A PRESS of MODE_BUTTON moves to the next mode:
     *         NORMAL -> TEST -> OFF -> NORMAL
     *     Every other event leaves the mode as it is.
     *
     * TODO step B - actions:
     *     entering TEST: all LEDs on
     *     entering OFF:  all LEDs off
     *     in NORMAL:     events of the other buttons go to rules_apply()
     *     in TEST, OFF:  the other buttons do nothing
     */

    /* '(void)x;' only silences the "unused parameter" warning.
     * Delete these two lines once you use button and event. */
    (void)button;
    (void)event;
}

void mode_poll_all(void)
{
    /* TODO step C:
     *     poll every button once (button_poll) and pass each event
     *     that is not NONE to mode_handle_event(). */
}
