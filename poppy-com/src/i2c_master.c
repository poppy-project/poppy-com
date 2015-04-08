#include <util/twi.h>
#include <avr/interrupt.h>
#include "poppy-com/inc/i2c_master.h"

// I2C Master mode

unsigned char i2cStart(unsigned char addr) {
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != 0x08)
    return 1;

    TWDR = addr;
    TWCR |= (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    switch (TWSR & 0xF8) {
        case 0x18:
        // ACK received
        return 0;
        break;
        case 0x20:
        // NOT ACK received
        return 1;
        break;
        case 0x38:
        // Arbitration lost
        return 1;
        break;
        default:
        return 1;
        break;
    }
}

void i2cStop(void) {
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

unsigned char i2cWrite(unsigned char data) {
    if (TWCR & (1<<TWINT)) {
        TWDR = data;
        TWCR = (1<<TWINT) | (1<<TWEN);
        while ((TWCR & (1<<TWINT)) == 0);
    } else {
        return 1;
    }
    return 0;
}

unsigned char i2cRead(unsigned char ack_enable) {
    if (ack_enable)
        TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    else
        TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}
