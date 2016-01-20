/*
 * poppyNetwork.c
 *
 * Created: 16/02/2015 11:53:28
 *  Author: nico
 *  Abstract: basics functionalities of the Poppy communication protocol
 */
#include "src/poppyNetwork.h"
#include "src/i2c_master.h"
#include "src/i2c_slave.h"
#include "src/context.h"
#include HAL

#include <Arduino.h>

extern context_t ctx;
//extern volatile unsigned char communicationState;

void poppyNetwork_ChangeHardwareMode(hardwareMode_t newMode) {
    ctx.hardMode = newMode;
}

// Startup and network configuration
void poppyNetwork_init(TX_CB tx_cb,
                       RX_CB rx_cb,
                       RX_CB rxgc_cb) {
    hal_init();

    // Initialization for UART mode
    Serial.begin(1000000);  // 1M clock
    Serial.setTimeout(1);   // 1ms timeout
    //communicationState = 0x00;

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
    // Module communication type
    ctx.hardMode = DEFAULTHARDWAREMODE;

    // Status
    ctx.status = (status_t) {.rx_error = FALSE,
                             .master_write = FALSE,
                             .unexpected_state = FALSE,
                             .warning = FALSE};
}

unsigned char poppyNetwork_read(unsigned char addr, msg_t *msg,
                                unsigned char reply_size) {
    if(ctx.hardMode == I2C) {
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
    else   // UART mode
    {
        // Send request trame
        //while(communicationState!=0);
        //communicationState = 0x01;
        Serial.write((addr<<1)|0x01);           // Adress byte construct like in I2C protocol
        Serial.write(msg->reg);
        Serial.write(msg->size);
        Serial.write(msg->data,msg->size);

        // Receive answer
        Serial.write((addr<<1)|0x00);
        msg->size = Serial.readBytes(msg->data,reply_size);
        //communicationState = 0x00;
    }
}

unsigned char poppyNetwork_write(unsigned char addr, msg_t *msg) {
    if(ctx.hardMode == I2C)
    {
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
        i2cWrite(crc(&msg->data[0], msg->size));
        i2c_transmit(STOP);
        return 0;
    }
    else
    {
        // Send write trame
        //while(communicationState!=0);
        //communicationState = 0x01;
        Serial.write((addr<<1)|0x01);           // Adress byte construct like in I2C protocol
        Serial.write(msg->reg);
        Serial.write(msg->size);
        Serial.write(msg->data,msg->size);
        Serial.write(crc(msg->data,msg->size));
        //communicationState = 0x00;
    }
}
