#ifndef HAL_H_
#define HAL_H_

#include "poppy-com/inc/context.h"

typedef enum {
    // Send start condition
    START,
    // Send data with ACK enable
    DATA,
    // Send data with ACK disable
    DATA_NACK,
    // Send stop condition
    STOP
}com_state_t;

void hal_init(void);
void id_update(unsigned char id);

unsigned char i2cAddr(unsigned char addr, msg_dir_t dir);
unsigned char i2cWrite(unsigned char data);
unsigned char i2cRead(unsigned char ack_enable, unsigned char *data);
unsigned char i2c_transmit(com_state_t type);
unsigned char crc(unsigned char* data, unsigned char size);

#endif /* HAL_H_ */
