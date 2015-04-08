#include <util/twi.h>
#include <avr/interrupt.h>
#include "poppy-com/inc/i2c_slave.h"
#include "poppy-com/inc/context.h"


// Global variables
extern context ctx;

// I2C Slave mode
ISR(TWI_vect) {
    //  Test if there is really an interrupt
    if (TWCR&(1<<TWINT))
        twi_interrupt_handler();
}

void twi_interrupt_handler(void) {
    switch (TWSR) {
    // SLAVE TRANSMITTER MODE
         // Own address SLA+R received, ACK returned.
        case TW_ST_SLA_ACK :
        // Data transmitted, ACK received.
        case TW_ST_DATA_ACK:
            // Slave TX callback
            ctx.tx_cb(&TWDR);
            TWCR |= (1<<TWINT)|(1<<TWEA);
            break;

    // SLAVE RECEIVER MODE
        // Previously addressed with own SLA+W; ACK has been returned.
        case TW_SR_SLA_ACK :
        // Previously addressed with general call; ACK has been returned.
        case TW_SR_GCALL_ACK :
            TWCR |= (1<<TWINT)|(1<<TWEA);
        break;

        // Data has been received on SLA+W; ACK has been returned.
        case TW_SR_DATA_ACK :
            // Slave RX callback
            ctx.rx_cb(TWDR);
            TWCR |= (1<<TWINT)|(1<<TWEA);
        break;

        // Data has been received on general call;ACK has been returned.
        case TW_SR_GCALL_DATA_ACK :
            // Slave RX general call callback
            ctx.rxgc_cb(TWDR);
            TWCR |= (1<<TWINT)|(1<<TWEA);
        break;

    // OTHER
        case TW_BUS_ERROR :  // Error
        case TW_NO_INFO :  // Error
            TWCR |= (1<<TWINT)|(1<<TWSTO)|(1<<TWEA);
        break;
        default:
            TWCR |= (1<<TWINT)|(1<<TWEA);
        break;
    }
}
