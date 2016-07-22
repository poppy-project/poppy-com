#include <util/twi.h>
#include <avr/interrupt.h>
#include "hal/arduino_uno/hal.h"
#include "inc/i2c_slave.h"

// I2C Master mode

// Global variables
extern context_t ctx;

void hal_init(void) {
    // I2C
    TWBR = ((MAINCLOCK / SCLFREQ) - 16) / 2;
    TWSR &= ~(1<<TWPS1) & ~(1<<TWPS0);  // SetPrescaler divisor to 1
    TWAR = (0x0A << 1) | (1<<TWGCE);  // I2C Address and enable general call
    TWAMR = 0x00;  // Not used
    TWCR = ((1 << TWEA) | (1 << TWEN) | (1 << TWIE));  // Enable ACK system
}

unsigned char i2c_transmit(com_state_t type) {
    switch (type) {
        case START:
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        break;
        case DATA:
            TWCR = (1 << TWINT) | (1 << TWEN) | (1<<TWEA);
        break;
        case DATA_NACK:
            TWCR = (1 << TWINT) | (1 << TWEN);
        break;
        case STOP:
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
            return 0;
    }
    // Wait for TWINT flag set in TWCR Register
    while (!(TWCR & (1 << TWINT)));
    // TODO(NR) Timeout
    // Return TWI Status Register, mask the prescaler bits (TWPS1,TWPS0)
    return (TWSR & 0xF8);
}


unsigned char i2cAddr(unsigned char addr, msg_dir_t dir) {
    unsigned char status;
    unsigned char n = 0;
    if (dir)
        addr = (addr << 1) | TW_READ;
    else
        addr = (addr << 1) | TW_WRITE;
    i2c_retry:
    if (n++ >= MAX_TRIES) return 1;
    status = i2c_transmit(START);
    if ((status != TW_START) & (status != TW_REP_START)) {
        ctx.status.master_write = TRUE;
        return 1;
    }

    TWDR = addr;
    switch (i2c_transmit(DATA)) {
        case TW_MT_SLA_ACK:
        case TW_MR_SLA_ACK:
            // ACK received
            return 0;
        break;
        case TW_MR_SLA_NACK:
        case TW_MT_SLA_NACK:
            // NACK received
        case TW_MT_ARB_LOST:
            // Arbitration lost
            goto i2c_retry;
        break;
        default:
            return 1;
        break;
    }
}

unsigned char i2cWrite(unsigned char data) {
    if (TWCR & (1<<TWINT)) {
        TWDR = data;
        if (i2c_transmit(DATA) != TW_MT_DATA_ACK) {
            ctx.status.master_write = TRUE;
            return 1;
        }
    } else {
        ctx.status.master_write = TRUE;
        return 1;
    }
    return 0;
}

unsigned char i2cRead(unsigned char ack_enable, unsigned char *data) {
    if (ack_enable) {
        if (i2c_transmit(DATA) != TW_MR_DATA_ACK) {
            ctx.status.master_read = TRUE;
            return 1;
        }
    } else {
        if (i2c_transmit(DATA_NACK) != TW_MR_DATA_NACK) {
            ctx.status.master_read = TRUE;
            return 1;
        }
    }
    *data = TWDR;
    return 0;
}

// I2C Slave mode
ISR(TWI_vect) {
    //  Test if there is really an interrupt
    if (TWCR&(1<<TWINT))
        switch (TWSR) {
        // SLAVE TRANSMITTER MODE
            case TW_ST_SLA_ACK:   // 0xA8: SLA+R received, ACK returned.
            case TW_ST_DATA_ACK:  // 0xB8: Data transmitted, ACK received.
                // Slave TX callback
                ctx.data_cb(TX, &TWDR);
                TWCR |= (1<<TWINT);
                break;

        // SLAVE RECEIVER MODE
            // Previously addressed with own SLA+W; ACK has been returned.
            case TW_SR_SLA_ACK:
            // Previously addressed with general call; ACK has been returned.
            case TW_SR_GCALL_ACK:
                TWCR |= (1<<TWINT)|(1<<TWEA);
            break;

            // Data has been received on SLA+W; ACK has been returned.
            case TW_SR_DATA_ACK:
                // Slave RX callback
                ctx.data_cb(RX, &TWDR);
                TWCR |= (1<<TWINT);
            break;

            // Data has been received on general call;ACK has been returned.
            case TW_SR_GCALL_DATA_ACK:
                // Slave RX general call callback
                ctx.data_cb(RXGC, &TWDR);
                TWCR |= (1<<TWINT);
            break;

        // OTHER
            case TW_BUS_ERROR:  // Error
            case TW_NO_INFO:  // Error
                TWCR |= (1<<TWINT)|(1<<TWSTO);
                ctx.status.unexpected_state = TRUE;
                ctx.data_cb = idle;
            break;
            case TW_SR_STOP:
                TWCR |= (1<<TWINT)|(1<<TWSTO);
                ctx.data_cb = idle;
                ctx.data_cb(END, &TWDR);
            default:
                TWCR |= (1<<TWINT);
            break;
        }
}

void id_update(unsigned char id) {
    ctx.id = id;
    TWAR = (ctx.id << 1) | (1 << TWGCE);
    /*
     * TORO(NR) : Write this ID on EEPROM and use it as default at
     *            reboot (do this after debug)
    */
}

unsigned char crc(unsigned char* data, unsigned char size) {
    unsigned char x;
    unsigned int crc = 0xFFFF;

    while (size--) {
        x = crc >> 8 ^ *data++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned int)(x << 12))
                         ^ ((unsigned int)(x <<5))
                         ^ ((unsigned int)x);
    }
    return (unsigned char)crc;
}
