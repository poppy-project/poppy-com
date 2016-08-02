/*
 * discovery_test.c
 *
 * Created: 27/07/2016 16:45:56
 * Author : Xevel
 */ 

#include "common.h"




int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
    board_init();

    /* This skeleton code simply sets the LED to the state of the button. */
    while (1) {
        /* Is button pressed? */
        if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
            /* Yes, so turn LED on. */
            ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
            } else {
            /* No, so turn LED off. */
            ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
        }
    }

}
