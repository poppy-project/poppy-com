#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

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

uint8_t i2cAddr(uint8_t addr, msg_dir_t dir);
uint8_t i2cWrite(uint8_t data);
uint8_t i2cRead(uint8_t ack_enable);
uint8_t i2c_transmit(com_state_t type);

uint8_t set_extern_id(uint8_t addr, uint8_t newid);
uint8_t get_extern_module_type(uint8_t addr, uint8_t *module_type);



#endif /* I2C_MASTER_H_ */
