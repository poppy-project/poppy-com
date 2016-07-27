#include <util/twi.h>
#include <avr/interrupt.h>
#include "atmega328p/hal.h"
#include "reception.h"

// I2C Master mode

// Global variables
extern context_t ctx;

/**
 * \fn void hal_init(void)
 * \brief I2C hardware configuration (clock, address, ACK system...)
 */
void hal_init(void) {
    // I2C settings
    // SetPrescaler divisor
    // Set I2C Address
    // Enable general call
    // Enable ACK system
}

/**
 * \fn unsigned char hal_transmit(com_state_t type)
 * \brief Manage I2C transmit case
 *
 * \param type byte type to transmit (Start, Data, Data NAK, or Stop)
 *
 * \return hardware status
 */
unsigned char hal_transmit(com_state_t type) {
    switch (type) {
        case START:
            // Send start condition
        break;
        case DATA:
            // Send data ACK required
        break;
        case DATA_NACK:
            // Send data no ACK required
        break;
        case STOP:
            // Send stop condition
            return 0;
    }
    // Wait the end of the transmition (here you can manage a timeout)
    // Manage the end of IRQ
    // Return the status
    return (/*status*/);
}

/**
 * \fn unsigned char hal_addr(unsigned short addr, msg_dir_t dir)
 * \brief send address byte
 *
 * \param addr device address
 * \param dir set message direction (read or write)
 *
 * \return error
 */
unsigned char hal_addr(unsigned short addr, msg_dir_t dir) {
    unsigned char status;
    unsigned char n = 0;
    addr = (addr << 1) | dir;
    i2c_retry:
    if (n++ >= MAX_TRIES) return 1;
    status = hal_transmit(START);
    if ((status != TW_START) & (status != TW_REP_START)) {
        ctx.status.master_write = TRUE;
        return 1;
    }

    // Send addr
    switch (hal_transmit(DATA)) {
        case // ACK received
            return 0;
        break;
        case // NACK received
        case // Arbitration lost
            goto i2c_retry;
        break;
        default:
            return 1;
        break;
    }
}

/**
 * \fn unsigned char hal_write(unsigned char data)
 * \brief write a data byte
 *
 * \param data data byte to send
 *
 * \return error
 */
unsigned char hal_write(unsigned char data) {
    if /*hardware ready*/ {
        // Put data into the dedicated register
        if (hal_transmit(DATA) /*== NO_ACK*/) {
            ctx.status.master_write = TRUE;
            return 1;
        }
    } else {
        ctx.status.master_write = TRUE;
        return 1;
    }
    return 0;
}

/**
 * \fn unsigned char hal_read(unsigned char ack_enable, unsigned char *data)
 * \brief read a data byte
 *
 * \param ack_enable ACK or not?
 * \param *data data byte catched
 *
 * \return error
 */
unsigned char hal_read(unsigned char ack_enable, unsigned char *data) {
    if (ack_enable) {
        if (hal_transmit(DATA) /*== NO_ACK*/) {
            ctx.status.master_read = TRUE;
            return 1;
        }
    } else {
        if (hal_transmit(DATA_NACK) /*== ACK*/) {
            ctx.status.master_read = TRUE;
            return 1;
        }
    }
    *data = /*data register*/;
    return 0;
}

// I2C interrupt management
ISR(/*vector*/) {
    //  Test if there is really an interrupt
    if (/*everything's good*/)
        switch (/*status*/) {
        // SLAVE TRANSMITTER MODE
            case //Slave TX
                ctx.data_cb(TX, &/*data register*/);
                break;

        // SLAVE RECEIVER MODE
            case /*address match*/:
            case /*general call*/:
                /*do your needed things*/
            break;

            // Data has been received on SLA+W; ACK has been returned.
            case /*data ACKed*/:
                ctx.data_cb(RX, &/*data register*/);
            break;

            // Data has been received on general call;ACK has been returned.
            case /*general call data ACKed*/:
                ctx.data_cb(RXGC, &/*data register*/);
            break;

        // OTHER
            case /*Error*/:
                ctx.status.unexpected_state = TRUE;
                ctx.data_cb = idle;
            break;
            case /*Stop*/:
                ctx.data_cb = idle;
                ctx.data_cb(END, &/*data register*/);
            default:
            break;
        }
}

/**
 * \fn void id_update(unsigned char id)
 * \brief update the slave address
 *
 * \param id new address
 *
 */
void id_update(unsigned char id) {
    ctx.id = id;
    /*address register*/ = (ctx.id << 1);
    // Write your ID on EEprom
}
