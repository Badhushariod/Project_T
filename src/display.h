#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

/*
 * Print a port value on one line, bit 7 first (on the left).
 *
 *   value 0x25  ->  PORT = 00100101
 *
 * Printing lives here, NOT in port.c: the port module only does logic,
 * so it stays easy to test automatically.
 */
void display_show(uint8_t value);

#endif /* DISPLAY_H */
