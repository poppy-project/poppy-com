#include "extras/hal/stub/hal.h"
#include "src/i2c_slave.h"

typedef enum {
    DATA_TX,
    ADDR_MATCH,
    DATA_RX,
    DATA_RXGC,
    STOP_CATCHED,
}status_reg_t;

// Global variables
extern context_t ctx;
unsigned char data_reg;
status_reg_t status_reg;
unsigned char gc = 0;
msg_dir_t dir_cpy;


// I2C Slave mode
void i2c_interrupt(void) {
    //  Test if there is really an interrupt
    switch (status_reg) {
    // SLAVE TRANSMITTER MODE
        case DATA_TX:
            // Slave TX callback
            ctx.data_cb(TX, &data_reg);
            break;

    // SLAVE RECEIVER MODE
        case ADDR_MATCH:
        break;

        // Data has been received on SLA+W; ACK has been returned.
        case DATA_RX:
            // Slave RX callback
            ctx.data_cb(RX, &data_reg);
        break;

        // Data has been received on general call;ACK has been returned.
        case DATA_RXGC:
            // Slave RX general call callback
            ctx.data_cb(RXGC, &data_reg);
        break;

    // OTHER
        case STOP_CATCHED:
            ctx.data_cb = idle;
            ctx.data_cb(END, &data_reg);
        default:
        break;
    }
}

void hal_init(void) {
    // I2C configuration
}

unsigned char i2c_transmit(com_state_t type) {
    switch (type) {
        case START:
            status_reg = ADDR_MATCH;
            i2c_interrupt();
        break;
        case DATA:
        case DATA_NACK:
            if (!dir_cpy) {
                if (gc) {
                    status_reg = DATA_RXGC;
                    i2c_interrupt();
                } else {
                    status_reg = DATA_RX;
                    i2c_interrupt();
                }
            } else {
                status_reg = DATA_TX;
                i2c_interrupt();
            }
        break;
        case STOP:
            status_reg = STOP_CATCHED;
            i2c_interrupt();
            return 0;
    }
    // TODO(NR) Timeout
    return (0);
}


unsigned char i2cAddr(unsigned char addr, msg_dir_t dir) {
    if (addr)
        gc = 0;
    else
        gc = 1;
    dir_cpy = dir;
    if (dir) {
        addr = (addr << 1) | 0;
    } else {
        addr = (addr << 1) | 1;
    }

    i2c_transmit(START);
    return 0;
}

unsigned char i2cWrite(unsigned char data) {
    data_reg = data;
    i2c_transmit(DATA);
    return 0;
}

unsigned char i2cRead(unsigned char ack_enable, unsigned char *data) {
    if (ack_enable)
        i2c_transmit(DATA);
    else
        i2c_transmit(DATA_NACK);
    *data = data_reg;
    return 0;
}

void id_update(unsigned char id) {
    ctx.id = id;
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
