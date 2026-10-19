#include "port.h"

/*
 * The simulated port register.
 * 'static' outside a function means: private to this file.
 * Other files can only change it by calling the functions below.
 */
static uint8_t port_out;

void port_init(void)
{
    port_out = 0;
}

uint8_t port_get(void)
{
    return port_out;
}

void port_set_pin(uint8_t pin)
{
    /* TODO: set bit 'pin' of port_out to 1. */
    (void)pin;   /* delete this line when you write your code */
}

void port_clear_pin(uint8_t pin)
{
    /* TODO: clear bit 'pin' of port_out to 0. */
    (void)pin;   /* delete this line when you write your code */
}

void port_toggle_pin(uint8_t pin)
{
    /* TODO: flip bit 'pin' of port_out. */
    (void)pin;   /* delete this line when you write your code */
}

bool port_read_pin(uint8_t pin)
{
    /* TODO: return true if bit 'pin' of port_out is 1, false if it is 0. */
    (void)pin;   /* delete this line when you write your code */
    return false;
}
