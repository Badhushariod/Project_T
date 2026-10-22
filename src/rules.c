#include <stddef.h>
#include "rules.h"
#include "port.h"

/*
 * The rule table: an array of structs.
 * 'const' = never changes. On a real MCU, a const table like this
 * is stored in flash instead of RAM.
 */
static const rule_t rules[] = {
    /* button  event                  action             led */
    {  0,      BUTTON_EVENT_PRESSED,  ACTION_LED_TOGGLE, 0   },  /* button 0 toggles LED 0         */
    {  1,      BUTTON_EVENT_PRESSED,  ACTION_LED_ON,     1   },  /* LED 1 is on while button 1 ... */
    {  1,      BUTTON_EVENT_RELEASED, ACTION_LED_OFF,    1   },  /* ... is held down               */
    {  2,      BUTTON_EVENT_PRESSED,  ACTION_LED_TOGGLE, 7   },  /* button 2 toggles LED 7         */
};

/* Number of rules = size of the whole array / size of one rule. */
#define RULE_COUNT (sizeof rules / sizeof rules[0])

void rules_apply(uint8_t button, button_event_t event)
{
    for (size_t i = 0; i < RULE_COUNT; i++) {
        
        if (button == rules[i].button && event ==rules[i].event)
        {
            led_action_t action = rules[i].action;
            uint8_t  led = rules[i].led ;
            if (action == ACTION_LED_ON){
                port_set_pin(led);
            }
            else if (action == ACTION_LED_OFF){
                port_clear_pin(led);
            }
            else{
                port_toggle_pin(led);
            }
        }
    }
}

void rules_poll_all(void)
{
    /* TODO 3: for every button 0..BUTTON_COUNT-1:
     *           ask button_poll() for its event;
     *           if the event is not BUTTON_EVENT_NONE, call rules_apply(). */
    for (size_t i = 0; i < BUTTON_COUNT; i++){
        button_event_t a= button_poll(i);
        if (a != BUTTON_EVENT_NONE){
            rules_apply(i,a);
        }
        

    }
}
