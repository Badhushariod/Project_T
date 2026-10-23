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

    if (button == MODE_BUTTON && event == BUTTON_EVENT_PRESSED)
    {
        switch(mode){
            case MODE_NORMAL :
                mode = MODE_TEST ;   
                for (uint8_t i=0;i<PORT_PIN_COUNT;i++){
                    port_set_pin(i);
                }           
                break;
            case (MODE_TEST):
                mode = MODE_OFF;
                for (uint8_t i=0;i<PORT_PIN_COUNT;i++){
                    port_clear_pin(i);
                }     
                break;
            case MODE_OFF :
                mode = MODE_NORMAL;
                break;
            
        }
    }
    else {
        if (mode == MODE_NORMAL)
        {
            rules_apply(button,event);
        }
    }


}

void mode_poll_all(void)
{
    /* TODO step C:
     *     poll every button once (button_poll) and pass each event
     *     that is not NONE to mode_handle_event(). */
    for (size_t i = 0; i < BUTTON_COUNT; i++){      /* every button */
        button_event_t a= button_poll(i);           /* what happened?*/
        if (a != BUTTON_EVENT_NONE){                /* something happened  */
            mode_handle_event(i,a);                       /* → do what the table says */
        }
        

    }
}
