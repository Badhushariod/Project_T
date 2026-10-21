#include "input.h"

/*
 * The simulated input register.
 * 'static' outside a function means: private to this file.
 */
static uint8_t input_reg;

void input_init(void)
{
    input_reg = 0xFF;   /* all pins pulled up: 1111 1111 */
}

uint8_t input_get(void)
{
    return input_reg;
}

bool input_press(uint8_t pin)
{
    if (pin >= INPUT_PIN_COUNT )
    {
        return false;
    }
    input_reg &= ~(1u << pin);
    return true; 
}

bool input_release(uint8_t pin)
{
    if (pin >= INPUT_PIN_COUNT )
    {
        return false;
    }
    input_reg |= (1u << pin);
    return true;
}


bool input_is_pressed(uint8_t pin)
{

    if (pin >= INPUT_PIN_COUNT )
    {
        return false;
    }
    if ( (input_reg & (1u << pin)) == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}
