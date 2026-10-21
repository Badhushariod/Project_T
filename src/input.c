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
    /* TODO: invalid pin -> return false.
     *       Otherwise make bit 'pin' 0 (pressed) and return true. */
    (void)pin;   /* delete this line when you write your code */
    return false;
}

bool input_release(uint8_t pin)
{
    /* TODO: invalid pin -> return false.
     *       Otherwise make bit 'pin' 1 (released) and return true. */
    (void)pin;   /* delete this line when you write your code */
    return false;
}

bool input_is_pressed(uint8_t pin)
{
    /* TODO: invalid pin -> return false.
     *       Otherwise return true if bit 'pin' is 0 (pressed). */
    (void)pin;   /* delete this line when you write your code */
    return false;
}
