#ifndef I2C_SLAVE_H_
#define I2C_SLAVE_H_

#include "poppy-com/inc/context.h"

// Callbacks
void idle(msg_dir_t dir, volatile uint8_t data);
void get_size(msg_dir_t dir, volatile uint8_t data);
void get_data(msg_dir_t dir, volatile uint8_t data);

void msg_complete(msg_dir_t dir);

#endif /* I2C_SLAVE_H_ */
