#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>

/*
 * Simulated 8-bit input port with 8 buttons (pins 0..7).
 *
 * Wiring, like most real boards: each button connects its pin to GND,
 * and a pull-up resistor holds the pin HIGH while the button is not pressed.
 *
 *   button released  ->  pin reads 1   (pulled up)
 *   button pressed   ->  pin reads 0   (shorted to GND)      = "active low"
 *
 * Two kinds of functions:
 *   - WORLD side:    input_press / input_release simulate a finger on a button.
 *                    Real firmware cannot do this; only the outside world can.
 *   - FIRMWARE side: input_get / input_is_pressed only READ the port,
 *                    like real firmware reading an input register.
 */

#define INPUT_PIN_COUNT 8u              /* valid pins are 0 .. INPUT_PIN_COUNT-1 */

void    input_init(void);               /* no button pressed: all pins 1 (1111 1111) */
uint8_t input_get(void);                /* the raw 8-bit input register              */

/* WORLD side. Return false (and change nothing) for an invalid pin. */
bool    input_press(uint8_t pin);       /* finger down: pin becomes 0                */
bool    input_release(uint8_t pin);     /* finger up:   pin becomes 1                */

/* FIRMWARE side. */
bool    input_is_pressed(uint8_t pin);  /* true if the pin reads 0;
                                           false if it reads 1 or 'pin' is invalid   */

#endif /* INPUT_H */
