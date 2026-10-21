#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} Time_day;

int main(void){
    Time_day x;
    x.hour = 9;
    x.minute = 45;
    x.second = 00;
    printf ("%d:%d:%d\n",x.hour,x.minute,x.second );
}