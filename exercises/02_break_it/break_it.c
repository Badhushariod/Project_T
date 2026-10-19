/*
 * Experiment: what happens if we ask for an LED that does not exist?
 * The port has 8 LEDs: pins 0..7.
 */
#include <stdio.h>
#include "port.h"

int main(void)
{
    port_init();

    port_set_pin(2);
    printf("after set pin 2: port = %d\n", port_get());

    port_set_pin(8);                         /* there is no LED 8! */
    printf("after set pin 8: port = %d\n", port_get());

    return 0;
}
