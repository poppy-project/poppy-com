#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#include "src/context.h"

unsigned char set_extern_id(unsigned char addr, unsigned char newid);
unsigned char get_extern_module_type(unsigned char addr, unsigned char *module_type);



#endif /* I2C_MASTER_H_ */
