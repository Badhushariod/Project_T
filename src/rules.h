#ifndef RULES_H
#define RULES_H

#include <stdint.h>
#include "button.h"

/* What a rule does to an LED. */
typedef enum {
    ACTION_LED_ON,
    ACTION_LED_OFF,
    ACTION_LED_TOGGLE
} led_action_t;

/*
 * One rule:  "when <button> has <event>, do <action> on <led>".
 * A struct groups several values that belong together into one thing.
 */
typedef struct {
    uint8_t        button;   /* which button, 0..7          */
    button_event_t event;    /* PRESSED or RELEASED         */
    led_action_t   action;   /* ON, OFF or TOGGLE           */
    uint8_t        led;      /* which LED, 0..7             */
} rule_t;

void rules_apply(uint8_t button, button_event_t event);  /* run every rule that matches */
void rules_poll_all(void);                               /* poll every button and apply
                                                            the rules for each event     */

#endif /* RULES_H */
