/* 
Q1.
    Green + Button = Yellow        Green + Tick     = Ignore
    Yellow + Tick  = Red           Yellow + Button  = Ignore
    Red + Tick     = Green         Red + Button     = Ignore
    Unknown + Tick = Red           Unknown + Button = Red

Q2.
    .h = enums + prototypes
    .c = state + 4 funtions
Q3.
    get events, remember light(Static) , changes light 
Q4.
    switch light if event triggers it else ignore , default safe
*/

#include "traffic.h"

static light_t light;

void traffic_init(void){
    light = LIGHT_GREEN;
}

light_t traffic_get(void){
    return light;
}

void traffic_handle_event(traffic_event_t event){
    switch (light){
    case LIGHT_GREEN:
        if (event == EVENT_BUTTON){
            light = LIGHT_YELLOW;
        }
        break;
    case LIGHT_YELLOW:
        if (event == EVENT_TICK){
               light = LIGHT_RED;
        }
        break;
    case LIGHT_RED:
        if (event == EVENT_TICK){
            light = LIGHT_GREEN;
        }
        break;
    default:
        light = LIGHT_RED;
        break;
    }
}

void traffic_inject_fault(int raw_values){
    light = (light_t)raw_values;
}
