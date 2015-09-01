#include <avr/io.h>
#include "poppy-com/hal/atmega328p/halboot.h"

// Define your left pin alias
#define LEFTPIN PORTD1
// Define your right pin alias
#define RIGHTPIN PORTD0

// Global variables
extern context_t ctx;


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
