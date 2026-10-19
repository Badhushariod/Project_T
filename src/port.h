#ifndef PORT_H
#define PORT_H

#include <stdbool.h>
#include <stdint.h>

/*
 * Simulated 8-bit output port driving 8 LEDs.
 *
 *   bit number:  7 6 5 4 3 2 1 0
 *   LED number:  7 6 5 4 3 2 1 0     1 = LED on, 0 = LED off
 *
 * Every function changes ONLY the pin it is given. All other pins keep
 * their value.
 */

void    port_init(void);               /* all LEDs off: port = 0000 0000      */
void    port_set_pin(uint8_t pin);     /* LED on:  bit 'pin' becomes 1        */
void    port_clear_pin(uint8_t pin);   /* LED off: bit 'pin' becomes 0        */
void    port_toggle_pin(uint8_t pin);  /* flip bit 'pin': 1 -> 0 or 0 -> 1    */
bool    port_read_pin(uint8_t pin);    /* true if bit 'pin' is 1              */
uint8_t port_get(void);                /* the whole 8-bit port value          */

#endif /* PORT_H */
