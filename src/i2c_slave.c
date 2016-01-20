#include "src/i2c_slave.h"
#include HAL

// Global variables
extern context_t ctx;
/*
 * idle function is called when we are ready to receive or send a new message.
 */
void idle(msg_dir_t dir, volatile unsigned char *data) {
    static unsigned char *data_to_send;
    static unsigned char msg_size = 0;
    switch (dir) {
        case TX:
            /*
             * At this point we should have something ready to send.
             */
            if (msg_size) {
                /*
                 *This case is dedicated to protocol messages
                 */
                msg_size--;
                *data = *data_to_send++;
            } else {
                ctx.tx_cb(&ctx.msg);
            }
        break;
        case RX:
        case RXGC:
            /*
             * That should be a new message to receive.
             */
             ctx.msg.type = *data;
            switch (ctx.msg.type) {
                case GET_ID:
                    // Reply with ID
                    msg_size = 1;
                    data_to_send = &ctx.id;
                break;
                case WRITE_ID:
                    // Get and save a new given ID
                    ctx.msg.size = 1;
                    ctx.data_cb = get_data;
                break;
                case GET_MODULE_TYPE:
                    // Reply with module_type number
                    msg_size = 1;
                    data_to_send = &ctx.type;
                break;
                case GET_STATUS:
                    // Reply with a status register
                    msg_size = 1;
                    data_to_send = (unsigned char*)&ctx.status;
                    // TODO(NR) ca devrais reset le status...
                break;
                case GET_FIRM_REVISION:
                    // Reply with the actual firmware revision number
                    // TODO(NR)
                break;
                default:
                    ctx.data_cb = get_size;
                break;
            }
        break;
        case END:
            if (msg_size > 0)
                ctx.status.rx_error = TRUE;
            msg_size = 0;
        break;
    }
}

void get_size(msg_dir_t dir, volatile unsigned char *data) {
    ctx.msg.size = *data;
    ctx.data_cb = get_data;
}

void get_data(msg_dir_t dir, volatile unsigned char *data) {
    static unsigned char data_count = 0;
    if (data_count < ctx.msg.size) {
        ctx.msg.data[data_count++] = *data;
    } else {
        ctx.data_cb = idle;
        data_count = 0;
        if (*data == crc(&ctx.msg.data[0], ctx.msg.size))
            msg_complete(dir);
        else
            ctx.status.rx_error = TRUE;
    }
}

void msg_complete(msg_dir_t dir) {
    switch (ctx.msg.type) {
        case WRITE_ID:
            // Get and save a new given ID
            id_update(ctx.msg.data[0]);
        break;
        case GET_ID:
        case GET_MODULE_TYPE:
        case GET_STATUS:
        case GET_FIRM_REVISION:
            // ERROR
            ctx.status.rx_error = TRUE;
        break;
        default:
            if (dir == RX)
                ctx.rx_cb(dir, &ctx.msg);
            else
                ctx.rxgc_cb(dir, &ctx.msg);
        break;
    }
}
