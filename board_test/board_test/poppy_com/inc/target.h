#ifndef _TARGET_H_
#define _TARGET_H_

#include "context.h"

unsigned char multicast_target_bank(unsigned short val);
void add_multicast_target(unsigned short target);

unsigned char virtual_target_bank(unsigned short val);
void add_virtual_target(unsigned short target);


#endif /* _TARGET_H_ */
