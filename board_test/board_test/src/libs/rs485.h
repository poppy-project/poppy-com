/*
 * rs485.h
 *
 * Created: 31/07/2016 19:17:06
 *  Author: Xevel
 */ 


#ifndef RS485_H_
#define RS485_H_

void rs485_set_tx(bool enable);
void rs485_set_rx(bool enable);
bool rs485_init(void);

bool rs485_hal_init(void);

#endif /* RS485_H_ */