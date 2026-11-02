#ifndef TRAFFIC_H
#define TRAFFIC_H

/*ADD Enums*/
/*States*/
typedef enum{
    LIGHT_GREEN,
    LIGHT_YELLOW,
    LIGHT_RED,
}light_t;

/*eVENTS*/
typedef enum{
    EVENT_TICK,
    EVENT_BUTTON,
}traffic_event_t;


/*Funtion defenition*/

void traffic_init(void);
light_t traffic_get(void);
void traffic_handle_event(traffic_event_t event);
void traffic_inject_fault(int raw_values);

#endif /*TRAFFIC_H*/