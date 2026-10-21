/*
 * Demo for display_show(): compare each "actual" line with the
 * "expected" line above it.
 */
#include <stdio.h>
#include "port.h"
#include "display.h"

int main(void)
{
    port_init();
    printf("expected: PORT = 00000000   LEDs: [ . . . . . . . . ]\n");
    printf("actual:   ");
    display_show(port_get());

    port_set_pin(0);
    port_set_pin(2);
    port_set_pin(5);
    printf("\nexpected: PORT = 00100101   LEDs: [ . . * . . * . * ]\n");
    printf("actual:   ");
    display_show(port_get());

    port_set_pin(7);
    printf("\nexpected: PORT = 10100101   LEDs: [ * . * . . * . * ]\n");
    printf("actual:   ");
    display_show(port_get());

    return 0;
}
