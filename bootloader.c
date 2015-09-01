/*
 * banc_test1.c
 *
 * Created: 11/02/2015 12:40:48
 *  Author: Nicolas Rabault
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "poppy-com/inc/config.h"
#include HAL
#include HALBOOT
#include "poppy-com/poppyNetwork.h"

typedef enum {
    BOOTLOADER_WRITE_ID = PROTOCOL_REGISTER_NB,/*!< Get and save a new given ID. */
    BOOTLOADER_REGISTER_NB /*!< This is the minim register value available for applicative side. */
}boot_register_t;

volatile unsigned char token = 0;

void rx_cb(msg_dir_t dir, msg_t *msg) {
    if (msg->data[0] == 0xFE)
        // led on
        PORTB |= _BV(PORTB5);
    else
        // led off
        PORTB &= ~_BV(PORTB5);
}

void rxgc_cb(msg_dir_t dir, msg_t *msg) {
    switch (msg->reg) {
        case BOOTLOADER_WRITE_ID:
            if (token == 1) {
                id_update(msg->data[0]);
                token++;
            }
            break;
        default:
        break;
    }
}

void tx_cb(msg_t *msg) {
    msg->data[0] = 0xCA;
    msg->size = 1;
}

void Init(void) {
    // Disable interrupt
    cli();

    // input/output configuration
    DDRB = 0x10;
    DDRC = 0x30;
    DDRD = 0xF1;

    /* Set pin 5 of PORTB for arduino led output*/
    DDRB |= _BV(DDB5);

    // Clock configuration
    CLKPR |= (1<<CLKPCE);   // Enable modification of CLKPR register
    // Disable clk prescaling
    CLKPR &= (~(1 <<CLKPS0) & ~(1 <<CLKPS1) & ~(1 <<CLKPS2) & ~(1 <<CLKPS3));
    CLKPR &= ~(1<<CLKPCE);  // Disable modification of CLKPR register

    // Timer0 in 8 bits with no interrupt
    TCCR0B |= 0x03;  // Set up timer at Fcpu/64

    poppyNetwork_init(tx_cb, rx_cb, rxgc_cb);

    // enable interrupt
    sei();
}

void Timerms(unsigned int nb) {
    /*
     * Wait for a while just to be sure that all other modules are started.
     * To do that we use timer 0.
     */
    #if MAINCLOCK/100 <= 0xFF
        TCCR0B |= 0x01;
        unsigned int ticksCount = 255 - (MAINCLOCK/1000);
    #elif(MAINCLOCK/8)/1000 <= 0xFF
        TCCR0B |= 0x02;
        unsigned int ticksCount = 255 - ((MAINCLOCK/8)/1000);
    #elif(MAINCLOCK/64)/1000 <= 0xFF
        TCCR0B |= 0x03;
        unsigned int ticksCount = 255 - ((MAINCLOCK/64)/1000);
    #elif(MAINCLOCK/256)/1000 <= 0xFF
        TCCR0B |= 0x04;
        unsigned int ticksCount = 255 - ((MAINCLOCK/256)/1000);
    #elif(MAINCLOCK/1024)/1000 <= 0xFF
        TCCR0B |= 0x05;
        unsigned int ticksCount = 255 - ((MAINCLOCK/1024)/1000);
    #else
        #error *** Incompatible Clock set at MAINCLOCK. ***
    #endif
    /* Wait 10ms */
    unsigned int i;
    for (i = 0; i < nb; i++) {
        TCNT0 = ticksCount;
        while (TCNT0 >= ticksCount);
    }
}

void master(void) {
    Timerms(5);
    msg_t msg1 = {.reg = 0x55, .size = 1, .data[0] = 0xCA};

    msg_t msg2 = {.reg = 0x55, .size = 1, .data[0] = 0xFE};

    char i = 0;
    char nb_modul = 0;
    msg_t addr, type;
    unsigned char module_type;

    // Set right to 1
    PORTD |= _BV(PORTD1);

    addr.reg = BOOTLOADER_WRITE_ID;
    addr.size = 1;
    addr.data[0] = DEFAULTID+1;


    while (1) {
        if (poppyNetwork_write(0x00, &addr) == 1)
            break;
        if (get_extern_module_type(addr.data[0], &module_type) == 1)
            break;
        nb_modul++;
        addr.data[0] = DEFAULTID+1+(++i);
    }
    Timerms(5);

    while (1) {
        for (i = DEFAULTID+1; i <= (DEFAULTID+nb_modul); i++) {
            // Led on
            PORTB |= _BV(PORTB5);
            poppyNetwork_write(i, &msg2);
            Timerms(100);
            // Led off
            PORTB &= ~_BV(PORTB5);
            poppyNetwork_write(i, &msg1);
            Timerms(100);
        }
    }
}

int main(void) {
    Init();

    while (1) {
        if (token == 0) {
            if ((pin() == LEFT_ENABLE) || (pin() == RIGHT_ENABLE))
                token = 1;
        } else {
            if (token == 2) {
                // Pass the token
                pass_token();
            }
        }
    }
    return 0;
}
