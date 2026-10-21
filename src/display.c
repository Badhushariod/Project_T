#include <stdio.h>
#include "display.h"

void display_show(uint8_t value)
{
    printf("PORT = ");

    for (int pin = 7; pin>=0; pin --)
    {
        if (value & (1u<<pin))
        {
            printf("1");

        }
        else
        {
            printf("0");
        }
    }
    printf("   LEDs: [");
    for (int pin = 7; pin>=0; pin --)
    {
        if (value & (1u<<pin))
        {
            printf(" *");

        }
        else
        {
            printf(" .");
        }
    }
    printf(" ]");
    printf("\n");

}

    

