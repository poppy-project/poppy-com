/*
 * poppyNetwork.c
 *
 * Created: 16/02/2015 11:53:28
 *  Author: nico
 *  Abstract: basics functionalities of the Poppy communication protocol
 */

#include "poppyNetwork.h"
#include "i2c_master.h"
#include "context.h"

extern context ctx;

// Startup and network configuration
void poppyNetwork_init(TX_CB tx_cb,
                       RX_CB rx_cb,
                       RX_CB rxgc_cb) {
    /*
     * Set pin PB4 to 0.
     * This is used to detect if someone is plugged into the input.
     * Set pin PB5 to 0.
     * This is used to isolate the network input and output.
     */

    /* Set pin 5 of PORTB in output mode. */
    DDRB |= _BV(DDB5);
    /* Set pin 4 of PORTB in output mode. */
    DDRB |= _BV(DDB4);
    /* Set pin 5 low to isolate input and output. */
    PORTB &= ~_BV(PORTB5);
    /* Set pin 4 low to pull SDA low. */
    PORTB &= ~_BV(PORTB5);

    /*
     * Wait for a while just to be sure that all other modules are started.
     * To do that we use timer 0.
     */
#if MAINCLOCK/100 <= 0xFF
    TCCR0B |= 0x01;
    unsigned int ticksCount = 255 - (MAINCLOCK/100);
#elif(MAINCLOCK/8)/1000 <= 0xFF
    TCCR0B |= 0x02;
    unsigned int ticksCount = 255 - ((MAINCLOCK/8)/100);
#elif(MAINCLOCK/64)/1000 <= 0xFF
    TCCR0B |= 0x03;
    unsigned int ticksCount = 255 - ((MAINCLOCK/64)/100);
#elif(MAINCLOCK/256)/1000 <= 0xFF
    TCCR0B |= 0x04;
    unsigned int ticksCount = 255 - ((MAINCLOCK/256)/100);
#elif(MAINCLOCK/1024)/1000 <= 0xFF
    TCCR0B |= 0x05;
    unsigned int ticksCount = 255 - ((MAINCLOCK/1024)/100);
#else
    #error *** Incompatible Clock set at MAINCLOCK. Your setting is probably too speed. ***
#endif
    /* Wait 10ms */
    unsigned int i;
    for (i = 0; i < 10; i++) {
        TCNT0 = ticksCount;
        while (TCNT0 >= ticksCount);
    }

    /*
     * At this point all modules should have put SDA low, we are able to see if someone is plugged into our input.
     */
    /* Set SDA (pin 4 of PORTC) into input mode. */
    DDRC &= ~_BV(DDC4);
    /* Set SDA (pin 4 of PORTC) into tristated mode (hight-impedance). */
    PORTC = PORTC & ~_BV(DDC4);


    DDRC &= ~_BV(DDC5);
    PORTC = PORTC & ~_BV(DDC5);

    /* Read the value to know if a module is plugged before. */
    if (!(PORTC & _BV(DDC4))) {
        /*
         * You are the first module of the robot!
         * You have to initiate the auto addressing routine.
         */

        /* Enable led just for debug! */
         // PORTB |= _BV(PORTB5);
    } else {
        /*
         * You are not the first module of the robot!
         * Someone else will probably initiate the auto addressing routine.
         * You just have to wait your turn!
         */
    }

    // I2C
    TWBR = 0x02;  // Set SCL Frequency to 400Khz
    TWSR &= !(1<<TWPS1) & !(1<<TWPS0);  // SetPrescaler (for SCL frequency)
    TWAR = (0x0A << 1) & ~(1<<TWGCE);  // I2C Address
    TWAMR = 0x00;  // Not used
    TWCR = ((1 << TWEA) | (1 << TWEN) | (1 << TWIE));  // Active ACK system

    // Save context
    // Slave TX callback
    ctx.tx_cb = tx_cb;
    // Slave RX callback
    ctx.rx_cb = rx_cb;
    // Slave RX general call callback
    ctx.rxgc_cb = rxgc_cb;
}


uint8_t poppyNetwork_read(uint8_t addr,
                          uint8_t *data,
                          uint8_t size) {
    i2cStart((addr << 1) | 1);
    for (unsigned char i = 0; i < size; i++) {
        data[i] = i2cRead(1);
    }
    i2cStop();
    return 0;
}

uint8_t poppyNetwork_write(uint8_t addr,
                           uint8_t *data,
                           uint8_t size) {
    if (i2cStart(addr << 1)) {
        i2cStop();
        return 1;
    }
    // Write DATA
    for (unsigned char i = 0; i < size; i++) {
        if (i2cWrite (data[i]));
    }
    i2cStop();
    return 0;
}
