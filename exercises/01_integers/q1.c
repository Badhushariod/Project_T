#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint8_t a = 250, b = 10;
    uint8_t c = a + b;
    int     d = a + b;
    int          x = -1;
    unsigned int y = 1;

    printf("c = %d\n", c);
    printf("d = %d\n", d);
    printf("x < y is %s\n", (x < y) ? "true" : "false");
    printf("x as unsigned = %u (0x%X)\n", (unsigned int)x, (unsigned int)x);
    return 0;
}
