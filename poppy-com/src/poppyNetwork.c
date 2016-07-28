/*
 * poppyNetwork.c
 *
 * Created: 16/02/2015 11:53:28
 *  Author: nico
 *  Abstract: basics functionalities of the Poppy communication protocol
 */
#include "poppyNetwork.h"
#include "sys_msg.h"
#include "reception.h"
#include "context.h"
#include "hal.h"

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

unsigned char poppyNetwork_read(unsigned short target, msg_t *msg,
                                unsigned short reply_size) {
    unsigned char i = 0;

    // Write the command
    if (hal_addr(target, TX)) {
        hal_transmit(STOP);
        return 1;
    }
    if (hal_write(msg->msg_type)) {
        hal_transmit(STOP);
        return 1;
    }
    if (hal_write(msg->size)) {
        hal_transmit(STOP);
        return 1;
    }
    for (i = 0; i < msg->size; i++) {
        if (hal_write(msg->data[i])) {
            hal_transmit(STOP);
            return 1;
        }
    }

    // Read the reply
    if (hal_addr(target, RX)) {
        hal_transmit(STOP);
        return 1;
    }
    msg->size = reply_size;
    for (i = 0; i < msg->size; i++) {
        if (hal_read(FALSE, &msg->data[i])) {
            hal_transmit(STOP);
            return 1;
        }
    }
    hal_transmit(STOP);
    return 0;
}

unsigned char poppyNetwork_write(unsigned short target, msg_t *msg) {
    // Start and send Target field
    if (hal_addr(target, TX)) {
        hal_transmit(STOP);
        return 1;
    }
    // Write Source field
    hal_write(ctx.id);

    // Transform the message into a table
    unsigned char* stream = (unsigned char*) msg;
    // Send Msg_type, ACK_enable, and Msg_size.
    for (unsigned char i = 0; i < 3; i++) 
        hal_write(stream[i]);
    // Send data
    for (unsigned char i = 0; i < msg->size; i++) {
        hal_write(msg->data[i]);
    }
    unsigned short crc_val = crc(&msg->data[0], msg->size);
    hal_write((unsigned char)crc_val);
    hal_write((unsigned char)(crc_val << 8));
    hal_transmit(STOP);
    return 0;
}
