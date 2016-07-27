#ifndef _RECEPTION_H_
#define _RECEPTION_H_

#include "context.h"

void id_update(unsigned char id);
unsigned short crc(unsigned char* data, unsigned short size);

// Callbacks
void idle(msg_dir_t dir, volatile unsigned char *data);
void get_size(msg_dir_t dir, volatile unsigned char *data);
void get_data(msg_dir_t dir, volatile unsigned char *data);

void msg_complete(msg_dir_t dir);

#endif /* _RECEPTION_H_ */
