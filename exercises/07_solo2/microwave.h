#ifndef MICROWAVE_H
#define MICROWAVE_H
#include <stdbool.h>
/*States*/
typedef enum { MW_IDLE, MW_COOKING, MW_PAUSED } mw_state_t;
/*Event*/
typedef enum { MW_EVENT_START, MW_EVENT_DOOR_OPEN, MW_EVENT_TIMER_DONE } mw_event_t;

/*Function defenition*/
void microwave_init(void);         //start idle

mw_state_t microwave_get(void);    //Current State

void microwave_handle_event(mw_event_t event); //state machine

bool microwave_heater_on(void);       //only while cooking

void microwave_inject_fault(int raw_value);  //Test Only: currupt state

#endif /*MICROWAVE_H*/