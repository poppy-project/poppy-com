#include <avr/io.h>
#include "poppy-com/hal/atmega328p/halboot.h"

// Define your left pin alias
#define LEFTPIN PORTD1
// Define your right pin alias
#define RIGHTPIN PORTD0

// Global variables
extern context_t ctx;

void halboot_init(void) {

    /*
     * A normal bootloader init set left and right to input mode.
     * No starter  autodetection is needed. a specific bloc will start
     */

    // set right pin to in mode
    DDRD &= ~_BV(RIGHTPIN);

    // set left pin to out mode
    DDRD |= _BV(LEFTPIN);

    // Set Pull-up for right pin
    PORTD = 0x01;

    // Set left to 0
    PORTD &= ~_BV(LEFTPIN);
}

unsigned char pin(void) {
    static unsigned char pin = 0x00;
    /*
     * pin bit 0 is the left state
     * pin bit 1 is the right state
     *
     * Those bits can't be reset to 0
     */
    pin |= ((PIND & _BV(LEFTPIN)) > 0) << 0;
    pin |= ((PIND & _BV(RIGHTPIN)) > 0) << 1;
    return pin;
}

void pass_token(void) {
    if (pin() == LEFT_ENABLE) {
        // Set right to out mode
        DDRD |= _BV(RIGHTPIN);
        // Set right to 1
        PORTD |= _BV(RIGHTPIN);
    }
    if (pin() == RIGHT_ENABLE) {
        // Set left to out mode
        DDRD |= _BV(LEFTPIN);
        // Set left to 1
        PORTD |= _BV(LEFTPIN);
    }
}
