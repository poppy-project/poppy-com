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

context_t ctx;

// Startup and network configuration
void poppyNetwork_init(RX_CB rx_cb) {
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
    ctx.max_multicast_target = 0;
	
    // Status
    ctx.status = (status_t) {.rx_error = FALSE,
                             .unexpected_state = FALSE,
                             .warning = FALSE};
			 
	// Init hal
    hal_init();
}

unsigned char poppyNetwork_send(msg_t *msg) {

    // unsigned char* data = (unsigned char*)msg;
    unsigned short full_size = sizeof(header_t) + msg->header.size;
    unsigned short crc_val = 0;

    msg->header.protocol = PROTOCOL_REVISION;
    msg->header.source = ctx.id;
    crc_val = crc(msg->stream, full_size);
    full_size+=2;
    msg->data[msg->header.size] = (unsigned char)crc_val;
    msg->data[msg->header.size + 1] = (unsigned char)(crc_val >> 8);

    // Start and send Target field
    if (hal_transmit(msg->stream, full_size))
        return 1;
    if (msg->header.target_mode == IDACK) {
        ctx.data_cb = catch_ack;
        ctx.msg.ack = 0;
        // TODO
        // We could use the waituntil from the Xevel lib
        // In the same time if we don't have to do anything else we just can wait here...
        while (!ctx.msg.ack);
    }
    return 0;
}
