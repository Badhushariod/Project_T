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

bool port_set_pin(uint8_t pin)
{
    if (pin >= PORT_PIN_COUNT )
    {
        return false;
    }
    port_out |= (1u << pin);
    return true;
}

bool port_clear_pin(uint8_t pin)
{
    if (pin >= PORT_PIN_COUNT )
    {
        return false;
    }
    port_out &= ~(1u << pin);
    return true;
}

bool port_toggle_pin(uint8_t pin)
{
    if (pin >= PORT_PIN_COUNT )
    {
        return false;
    }
    port_out ^= (1u << pin);
    return true;
}

bool port_read_pin(uint8_t pin)
{

    if (pin >= PORT_PIN_COUNT )
    {
        return false;
    }
    if ( port_out & (1u << pin) )
    {
        return true;
    }
    else
    {
        return false;
    }

}
