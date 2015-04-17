#include <util/twi.h>
#include <avr/interrupt.h>
#include "poppy-com/inc/i2c_slave.h"


// Global variables
extern context_t ctx;

// I2C Slave mode
ISR(TWI_vect) {
    //  Test if there is really an interrupt
    if (TWCR&(1<<TWINT))
        switch (TWSR) {
        // SLAVE TRANSMITTER MODE
            case TW_ST_SLA_ACK:   // 0xA8: SLA+R received, ACK returned.
            case TW_ST_DATA_ACK:  // 0xB8: Data transmitted, ACK received.
                // Slave TX callback
                ctx.data_cb(TX, TWDR);
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
                ctx.data_cb(RX, TWDR);
                TWCR |= (1<<TWINT);
            break;

            // Data has been received on general call;ACK has been returned.
            case TW_SR_GCALL_DATA_ACK:
                // Slave RX general call callback
                ctx.data_cb(RXGC, TWDR);
                TWCR |= (1<<TWINT);
            break;

        // OTHER
            case TW_BUS_ERROR:  // Error
            case TW_NO_INFO:  // Error
                TWCR |= (1<<TWINT)|(1<<TWSTO);
                ctx.status.unexpected_state = TRUE;
                ctx.data_cb = idle;
            break;
            default:
                TWCR |= (1<<TWINT);
            break;
        }
}

/*
 * idle function is called when we are ready to receive or send a new message.
 */
void idle(msg_dir_t dir, volatile uint8_t data) {
    static uint8_t *data_to_send;
    static uint8_t msg_size = 0;
    switch (dir) {
        case TX:
            /*
             * At this point we should have something ready to send.
             */
            if (msg_size) {
                msg_size--;
                TWDR = *data_to_send;
                data_to_send++;
            } else {
                // Led on
                PORTB |= _BV(PORTB5);
                ctx.tx_cb(&ctx.msg);
            }
        break;
        case RX:
        case RXGC:
            /*
             * That should be a new message to receive.
             */
             ctx.msg.reg = data;
            switch (ctx.msg.reg) {
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
                    data_to_send = (uint8_t*)&ctx.status;
                    // TODO(NR) ca devrais reset le status...
                break;
                case GET_FIRM_REVISION:
                    // Reply with the actual firmware revision number
                    // TODO(NR)
                break;
                case WRITE_FIRMWARE:
                default:
                    ctx.data_cb = get_size;
                break;
            }
        break;
    }
}

void get_size(msg_dir_t dir, volatile uint8_t data) {
    ctx.msg.size = data;
    ctx.data_cb = get_data;
}

void get_data(msg_dir_t dir, volatile uint8_t data) {
    static uint8_t data_count = 0;
    ctx.msg.data[data_count++] = data;
    if (data_count == ctx.msg.size) {
        ctx.data_cb = idle;
        data_count = 0;
        msg_complete(dir);
        // In the future call the checksum function
    }
}

void msg_complete(msg_dir_t dir) {
    switch (ctx.msg.reg) {
        case WRITE_ID:
            // Get and save a new given ID
            ctx.id = ctx.msg.data[0];
            TWAR = (ctx.id << 1) & ~(1<<TWGCE);
            // TORO(NR) : Write this ID on EEPROM and use it as default at reboot (do this after debug)
        break;
        case WRITE_FIRMWARE:
            // TODO(NR)
        break;
        case GET_ID:
        case GET_MODULE_TYPE:
        case GET_STATUS:
        case GET_FIRM_REVISION:
            // ERROR
            ctx.status.rx_error = TRUE;
        break;
        default:
            ctx.rx_cb(dir, &ctx.msg);
        break;
    }
}
