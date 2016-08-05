#include "reception.h"
#include "hal.h"
#include "target.h"
#include "sys_msg.h"
#include "time.h"
//#include "test_board.h"

// Global variables
context_t ctx;
unsigned char drop = FALSE;
static timeout_t timeout;

/**
 * \fn unsigned char crc(unsigned char* data, unsigned char size)
 * \brief generate a CRC
 *
 * \param *data data table
 * \param size data size
 *
 * \return CRC value
 */
unsigned short crc(unsigned char* data, unsigned short size) {
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (size--) {
        x = crc >> 8 ^ *data++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned int)(x << 12))
                         ^ ((unsigned int)(x <<5))
                         ^ ((unsigned int)x);
    }
    return (unsigned short)crc;
}

/**
 * \fn void get_header(volatile unsigned char *data)
 * \brief catch a complete header
 *
 * \param *data byte received from serial
 */
void get_header(volatile unsigned char *data) {
    static unsigned short data_count = 0;
	//UP2;
	
	if (timeout_ended(&timeout)){
		data_count = 0;
		//UP1;DW1;
	}
	timeout_init(&timeout, 20);
    // Catch a byte.
    ctx.msg.header.unmap[data_count++] = *data;
//  	for ( int32_t i = -1; i< data_count ; i++   ){
//  		UP1;DW1;
//  	}
		

    // Check if we have all we need.
    if (data_count == (sizeof(header_t))) {
        // Reset the catcher.
        data_count = 0;

        // Find if we are concerned by this message.
        switch (ctx.msg.header.target_mode) {
            case ID:
            case IDACK:
            case TYPE:
            case BROADCAST:
                drop = !(ctx.msg.header.target == ctx.id ||
						ctx.msg.header.target == ctx.type ||
						ctx.msg.header.target == BROADCAST_VAL ||
						virtual_target_bank(ctx.msg.header.target));
						ctx.data_cb = get_data;
            break;
            case MULTICAST:
                drop = (multicast_target_bank(ctx.msg.header.target));
                ctx.data_cb = get_data;
            break;
            default:
				//DW2;
                return;
            break;
        }
    }
	//DW2;
}

/**
 * \fn void get_infos(volatile unsigned char *data)
 * \brief catch data field.
 *
 * \param *data byte received from serial
 */
void get_data(volatile unsigned char *data) {
    static unsigned short data_count = 0;
	
	if (timeout_ended(&timeout)){
		data_count = 0;
		drop = 0;
		ctx.data_cb = get_header;
		return;
	}
	timeout_init(&timeout, 20);

    ctx.msg.data[data_count] = *data;

    if (data_count > ctx.msg.header.size) {
		if (!drop) {
			ctx.msg.crc = ((unsigned short)ctx.msg.data[ctx.msg.header.size]) |
						  ((unsigned short)ctx.msg.data[ctx.msg.header.size + 1] << 8); 
			if (ctx.msg.crc == crc(ctx.msg.stream, ctx.msg.header.size + sizeof(header_t))) {
				if (ctx.msg.header.target_mode == IDACK) {
					send_ack();
				}
				ctx.data_cb = get_header;
				msg_complete();
			} else
				ctx.status.rx_error = TRUE;
		}
		ctx.data_cb = get_header;
		drop = 0;
        data_count = 0;
        return;
    }
    data_count++;
}

/**
 * \fn void catch_ack(volatile unsigned char *data)
 * \brief catch ack.
 *
 * \param *data byte received from serial
 */
void catch_ack(volatile unsigned char *data) {
    // Check ACK value.
    ctx.msg.ack = *data;
    // Set something (the Xevel thread thing??)...
}

/**
 * \fn void msg_complete($)
 * \brief the message is now complete, manage it.
 *
 * \param *data byte received from serial
 */
void msg_complete() {
    if (ctx.msg.header.target_mode == ID ||
        ctx.msg.header.target_mode == IDACK ||
        ctx.msg.header.target_mode == TYPE ||
        ctx.msg.header.target_mode == BROADCAST) {
        switch (ctx.msg.header.cmd) {
            case WRITE_ID:
                // Get and save a new given ID
                ctx.id = (((unsigned short)ctx.msg.data[1]) |
                          ((unsigned short)ctx.msg.data[0] << 8));
            break;
            case GET_ID:
            // call something...
            case GET_MODULE_TYPE:
            // call something...
            case GET_STATUS:
            // call something...
            case GET_FIRM_REVISION:
            // call something...
            break;
            default:
                    ctx.rx_cb(&ctx.msg);
            break;
        }
    } else {
        ctx.rx_cb(&ctx.msg);
    }
}
