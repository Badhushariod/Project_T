/*
Q1.
    idle + start =  cooking || idle + door_open = idle || idle + timer = idle
    cooking +start =cooking || cooking + door open = paused || cooking + timer = idle
    pause +start = cooking || pause + door_open = paused || pause + timer = paused
    unknown + any = idle

Q2. 
    .h = 2 enums 5 prototypes
    .c = state + 5 funtion

Q3.
    gets event, remember the state(static) , changes the state 

Q4
    cooking triggers at start when the device is at idle , if the door is open its hsows paused and then heter gets off , defauld condition is idel ,at timer the oven will be idle after cooking
*/

#include "microwave.h"

static mw_state_t state;

void microwave_init(void){
    state =  MW_IDLE;                    // start in IDLE 
}

mw_state_t microwave_get(void){
    return state;                       // current state
}

void microwave_handle_event(mw_event_t event){
    switch(state){
        case (MW_IDLE):
            if (event == MW_EVENT_START){
                state = MW_COOKING ;
            }
            break;
        case (MW_COOKING):
            if (event == MW_EVENT_DOOR_OPEN){
                state =  MW_PAUSED;
            }
            else if(event == MW_EVENT_TIMER_DONE){
                state = MW_IDLE ;
            }
            break;
        case (MW_PAUSED):
            if (event == MW_EVENT_START){
                state = MW_COOKING ;
            }
            break;
        default:
            state = MW_IDLE;
            break;
    }
}

bool microwave_heater_on(void){
    if (state == MW_COOKING){
        return true;
    }
    else {
        return false;
    }
}

void microwave_inject_fault(int raw_value){
    state = (mw_state_t) raw_value;
}