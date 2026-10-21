#include "button.h"

/*
 * What each button was at the last poll: true = pressed.
 * 'static' here: private to this file, and the values are kept
 * between calls (like 'static int count' in assessment Q5).
 */
static bool was_pressed[BUTTON_COUNT];

void button_init(void)
{
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        was_pressed[i] = false;
    }
}

button_event_t button_poll(uint8_t button)
{
    if (button >= BUTTON_COUNT) {
        return BUTTON_EVENT_NONE;
    }

    bool now = input_is_pressed(button);

    /* TODO: compare 'now' with was_pressed[button]:
     *         up   -> down : BUTTON_EVENT_PRESSED
     *         down -> up   : BUTTON_EVENT_RELEASED
     *         no change    : BUTTON_EVENT_NONE
     *       and remember 'now' for the next poll. */
    bool before = was_pressed[button];
    was_pressed[button] = now;
    if (!before && now){
        return BUTTON_EVENT_PRESSED;
    } 
    else if (before && !now)  {
        return BUTTON_EVENT_RELEASED;
    }
    else{
        return BUTTON_EVENT_NONE;
    }
}
