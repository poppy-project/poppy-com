#ifndef _HAL_H_
#define _HAL_H_

#include "context.h"

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

unsigned char hal_addr(unsigned short addr, msg_dir_t dir);
unsigned char hal_write(unsigned char data);
unsigned char hal_read(unsigned char ack_enable, unsigned char *data);
unsigned char hal_transmit(com_state_t type);

#endif /* _HAL_H_ */
