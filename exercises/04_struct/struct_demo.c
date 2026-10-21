#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} Time_day;

int main(void){
    Time_day x[3] = {
        {7,00,00},
        {12,30,00},
        {18,15,0},
    };
    for (int i=0 ; i<3 ; i++)  { 
        printf ("%d:%d:%d\n",x[i].hour,x[i].minute,x[i].second );
    }
}