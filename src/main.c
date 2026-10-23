/*
 * Project_T: simulated 8-LED device with a command line.
 * Type 'help' to see the commands.
 */
#include <stdio.h>
#include <string.h>
#include "port.h"
#include "display.h"
#include "input.h"
#include "rules.h"
#include "mode.h"


static void print_mode(void)
{
    device_mode_t current_mode = mode_get();
    switch (current_mode){
        case (MODE_OFF):
            printf(" Current mode is OFF\n");
            break;
        case (MODE_NORMAL):
            printf(" Current mode is NORMAL\n");
            break;
        case (MODE_TEST):
            printf(" Current mode is TEST\n");
            break;
        default :
            printf(" Current mode is Invalid\n");
            break;
    }
}

static void print_help(void)
{
    printf("commands:\n");
    printf("  set N      LED N on\n");
    printf("  clear N    LED N off\n");
    printf("  toggle N   flip LED N\n");
    printf("  read N     is LED N on?\n");
    printf("  press N    Finger Down\n");
    printf("  release N  Finger UP\n");
    printf("  mode       current mode\n");
    printf("  tick N     time\n");
    printf("  show       show all LEDs\n");
    printf("  help       this list\n");
    printf("  quit       exit\n");
    
}
static bool is_valid_pin(int pin)
{
    if (pin < (int)PORT_PIN_COUNT  && pin >=0)
    {
        return true;
    }
    else {
        return false;
    }
}

int main(void)
{
    char line[64];      /* one line typed by the user, e.g. "set 2\n"   */
    char cmd[16];       /* the first word of the line, e.g. "set"       */
    int  pin;           /* the number after the word, e.g. 2            */

    port_init();
    input_init();
    button_init();
    mode_init();

    printf("Project_T LED simulator. Type 'help' for commands.\n");

    while (1) {
        printf("> ");
        fflush(stdout);                 /* show "> " before waiting      */

        /* Read one line. NULL means there is no more input. */
        if (fgets(line, sizeof line, stdin) == NULL) {
            break;
        }

        /* Split the line into a word and a number.
         * 'pin' stays -1 if no number was typed. */
        pin = -1;
        if (sscanf(line, "%15s %d", cmd, &pin) < 1) {
            continue;                   /* empty line: ask again         */
        }

        if (strcmp(cmd, "quit") == 0) {
            break;
        } else if (strcmp(cmd, "help") == 0) {
            print_help();
        } else if (strcmp(cmd, "show") == 0){
            display_show(port_get());
        } else if (strcmp(cmd, "set") == 0){
            if (!is_valid_pin(pin)){
                printf("error: no pin %d (valid pins: 0-7)\n", pin);
            }
            else {
                port_set_pin(pin);
                display_show(port_get());
            }
        } else if (strcmp(cmd, "clear") == 0){
            if (!is_valid_pin(pin)){
                printf("error: no pin %d (valid pins: 0-7)\n", pin);
            }
            else {
                port_clear_pin(pin);
                display_show(port_get());
            }
        } else if (strcmp(cmd, "toggle") == 0){
            if (!is_valid_pin(pin)){
                printf("error: no pin %d (valid pins: 0-7)\n", pin);
            }
            else {
                port_toggle_pin(pin);
                display_show(port_get());
            }
        } else if (strcmp(cmd, "read") == 0){
            if (!is_valid_pin(pin)){
                printf("error: no pin %d (valid pins: 0-7)\n", pin);
            }
            else if (port_read_pin(pin)){
                    printf("LED %d is ON\n",pin);
                }
            else {
                    printf("LED %d is OFF\n",pin);
                }

        } else if (strcmp (cmd, "press") == 0){
            if (!is_valid_pin(pin)){
                printf("error: no pin %d (valid pins: 0-7)\n", pin);
            }
            else{
                input_press(pin);            // 1. finger down on button N               */
                mode_poll_all();            // 2. firmware checks the buttons and reacts */
                display_show(port_get());    // 3. look at the LEDs
            }
        } else if (strcmp (cmd, "release") == 0){
            if (!is_valid_pin(pin)){
                printf("error: no pin %d (valid pins: 0-7)\n", pin);
            }
            else{
                input_release(pin);           // 1. finger up on button N               */
                mode_poll_all();            // 2. firmware checks the buttons and reacts */
                display_show(port_get());    // 3. look at the LEDs
            }
        } else if(strcmp (cmd, "tick") == 0){
            if (pin<=20 && pin >=1){
                for (int i = 0; i < pin ; i++ ){
                    mode_poll_all();            // 2. firmware checks the buttons and reacts */
                    display_show(port_get());    // 3. look at the LEDs
                }
            }
            else {
                printf("provide a valid no. 1-20\n");
            }
        } else if (strcmp (cmd , "mode") ==0 ){
            print_mode();
        }
        else {
            printf("unknown command '%s' (type 'help')\n", cmd);
        }
    }

    printf("bye\n");
    return 0;
}
