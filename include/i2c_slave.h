#ifndef I2C_SLAVE_H_
#define I2C_SLAVE_H_

#include "context.h"

void id_update(unsigned char id);

// Callbacks
void idle(msg_dir_t dir, volatile unsigned char *data);
void get_size(msg_dir_t dir, volatile unsigned char *data);
void get_data(msg_dir_t dir, volatile unsigned char *data);

void msg_complete(msg_dir_t dir);

#endif /* I2C_SLAVE_H_ */
