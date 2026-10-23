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
            default:
                mode = MODE_OFF;
                for (uint8_t i=0;i<PORT_PIN_COUNT;i++){
                    port_clear_pin(i);
                }  
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
    for (size_t i = 0; i < BUTTON_COUNT; i++){      /* every button */
        button_event_t a= button_poll(i);           /* what happened?*/
        if (a != BUTTON_EVENT_NONE){                /* something happened  */
            mode_handle_event(i,a);                      
        

    }
}
}

/* FOR TESTS AND EXPERIMENTS ONLY (see mode.h). */
void mode_inject_fault(int raw_value)
{
    mode = (device_mode_t)raw_value;
}
