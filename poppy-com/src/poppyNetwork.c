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
void poppyNetwork_init(RX_CB rx_cb) {
    hal_init();

    // Save context
    // User side slave RX callback
    ctx.rx_cb = rx_cb;

    // Data callback
    ctx.data_cb = get_header;

    // Module id
    ctx.id = DEFAULTID;
    // Module type
    ctx.type = MODULETYPE;
    // TODO manage Alias
    ctx.max_extra_target = 0;

    // Status
    ctx.status = (status_t) {.rx_error = FALSE,
                             .unexpected_state = FALSE,
                             .warning = FALSE};
}

unsigned char poppyNetwork_send(msg_t *msg) {

    unsigned char* data = (unsigned char*)msg;
    unsigned short full_size = sizeof(header_t) + 1 + msg->header.size;

    msg->header.protocol = PROTOCOL_REVISION;
    msg->header.source = ctx.id;

    // Start and send Target field
    if (hal_transmit(data, full_size))
        return 1;
    if (msg->header.target_mode == IDACK) {
        ctx.data_cb = catch_ack;
        // TODO
        // We could use the waituntil from the Xevel lib
        // In the same time if we don't have to do anything else we just can wait here...
        ctx.msg.ack = 0;
        while (!ctx.msg.ack);
    }
    return 0;
}
