#include <util/twi.h>
#include <avr/interrupt.h>
#include "poppy-com/inc/i2c_master.h"

// I2C Master mode

// Global variables
extern context_t ctx;

uint8_t i2c_transmit(com_state_t type) {
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


uint8_t i2cAddr(uint8_t addr, msg_dir_t dir) {
    uint8_t status;
    uint8_t n = 0;
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

uint8_t i2cWrite(uint8_t data) {
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

uint8_t i2cRead(uint8_t ack_enable) {
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
    return TWDR;
}


// *****************************************************************************
uint8_t set_extern_id(uint8_t addr, uint8_t newid) {
    if (i2cAddr(addr, TX)) {
        i2c_transmit(STOP);
        return 1;
    }
    if (i2cWrite(WRITE_ID)) {
        i2c_transmit(STOP);
        return 1;
    }
    if (i2cWrite(newid)) {
        i2c_transmit(STOP);
        return 1;
    }
    i2c_transmit(STOP);
    return 0;
}

uint8_t get_extern_module_type(uint8_t addr, uint8_t *module_type) {
    if (i2cAddr(addr, TX)) {
        i2c_transmit(STOP);
        return 1;
    }
    if (i2cWrite(GET_MODULE_TYPE)) {
        i2c_transmit(STOP);
        return 1;
    }
    if (i2cAddr(addr, RX)) {
        i2c_transmit(STOP);
        return 1;
    }
    *module_type = i2cRead(FALSE);
    i2c_transmit(STOP);
    return 0;
}


 // // Reply with ID
 //    SET_ID,
 //    ping(uint8_t addr);
 //    // Reply with module_type number
 //    GET_MODULE_TYPE,
 //    // Reply with a status register
 //    GET_STATUS,
 //    // Reply with the actual firmware revision number
 //    GET_FIRM_REVISION,
