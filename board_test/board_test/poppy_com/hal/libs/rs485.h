/*
 * rs485.h
 *
 * Created: 31/07/2016 19:17:06
 *  Author: Xevel
 */ 


#ifndef RS485_H_
#define RS485_H_

#include "half_duplex.h"

void rs485_set_dir(half_duplex_dir_e dir);
void rs485_set_tx(bool enable);
void rs485_set_rx(bool enable);
bool rs485_init(void);

uint32_t rs485_write(uint32_t c);
uint32_t rs485_read(uint32_t* c);


bool rs485_hal_init(void);

#endif /* RS485_H_ */