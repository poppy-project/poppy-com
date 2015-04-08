#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

unsigned char i2cStart(unsigned char addr);
unsigned char i2cWrite(unsigned char data);
unsigned char i2cRead(unsigned char ack_enable);
void i2cStop(void);



#endif /* I2C_MASTER_H_ */
