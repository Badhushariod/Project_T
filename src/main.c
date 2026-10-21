/*
 * Project_T: simulated 8-LED device with a command line.
 * Type 'help' to see the commands.
 */
#include <stdio.h>
#include <string.h>
#include "port.h"
#include "display.h"

static void print_help(void)
{
    printf("commands:\n");
    printf("  set N      LED N on\n");
    printf("  clear N    LED N off\n");
    printf("  toggle N   flip LED N\n");
    printf("  read N     is LED N on?\n");
    printf("  show       show all LEDs\n");
    printf("  help       this list\n");
    printf("  quit       exit\n");
}

int main(void)
{
    char line[64];      /* one line typed by the user, e.g. "set 2\n"   */
    char cmd[16];       /* the first word of the line, e.g. "set"       */
    int  pin;           /* the number after the word, e.g. 2            */

    port_init();
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
            if (port_set_pin(pin)){
                display_show(port_get());
            }
            else {
                printf("error: no pin %d (valid pins: 0-7)\n", pin);
            }
        } else if (strcmp(cmd, "clear") == 0){
            if (port_clear_pin(pin)){
                display_show(port_get());
            }
            else {
                printf("error: no pin %d (valid pins: 0-7)\n", pin);
            }
        } else if (strcmp(cmd, "toggle") == 0){
            if (port_toggle_pin(pin)){
                display_show(port_get());
            }
            else {
                printf("error: no pin %d (valid pins: 0-7)\n", pin);
            }
        } 

        /* TODO: add your commands here: show, set, clear, toggle, read */
        else {
            printf("unknown command '%s' (type 'help')\n", cmd);
        }
    }

    printf("bye\n");
    return 0;
}
