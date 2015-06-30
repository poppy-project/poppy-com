/*
 * poppyNetwork.c
 *
 * Created: 16/02/2015 11:53:28
 *  Author: nico
 *  Abstract: basics functionalities of the Poppy communication protocol
 */
#include "poppy-com/poppyNetwork.h"
#include "poppy-com/inc/i2c_master.h"
#include "poppy-com/inc/i2c_slave.h"
#include "poppy-com/inc/context.h"
#include HAL

extern context_t ctx;

// Startup and network configuration
void poppyNetwork_init(TX_CB tx_cb,
                       RX_CB rx_cb,
                       RX_CB rxgc_cb) {
    hal_init();

    // Save context
    // User side slave TX callback
    ctx.tx_cb = tx_cb;
    // User side slave RX callback
    ctx.rx_cb = rx_cb;
    // User side slave RX general call callback
    ctx.rxgc_cb = rxgc_cb;

    // Data callback
    ctx.data_cb = idle;

    // Module id
    ctx.id = DEFAULTID;
    // Module type
    ctx.type = MODULETYPE;

    // Status
    ctx.status = (status_t) {.rx_error = FALSE,
                             .master_write = FALSE,
                             .unexpected_state = FALSE,
                             .warning = FALSE};
}

unsigned char poppyNetwork_read(unsigned char addr, msg_t *msg,
                                unsigned char reply_size) {
    unsigned char i = 0;

    // Write the command
    if (i2cAddr(addr, TX)) {
        i2c_transmit(STOP);
        return 1;
    }
    if (i2cWrite(msg->reg)) {
        i2c_transmit(STOP);
        return 1;
    }
    if (i2cWrite(msg->size)) {
        i2c_transmit(STOP);
        return 1;
    }
    for (i = 0; i < msg->size; i++) {
        if (i2cWrite(msg->data[i])) {
            i2c_transmit(STOP);
            return 1;
        }
    }

    // Read the reply
    if (i2cAddr(addr, RX)) {
        i2c_transmit(STOP);
        return 1;
    }
    msg->size = reply_size;
    for (i = 0; i < msg->size; i++) {
        if (i2cRead(FALSE, &msg->data[i])) {
            i2c_transmit(STOP);
            return 1;
        }
    }
    i2c_transmit(STOP);
    return 0;
}

unsigned char poppyNetwork_write(unsigned char addr, msg_t *msg) {
    if (i2cAddr(addr, TX)) {
        i2c_transmit(STOP);
        return 1;
    }
    // Write DATA
    i2cWrite(msg->reg);
    i2cWrite(msg->size);
    for (unsigned char i = 0; i < msg->size; i++) {
        i2cWrite(msg->data[i]);
    }
    i2c_transmit(STOP);
    return 0;
}
