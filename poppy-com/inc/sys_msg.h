#ifndef _SYS_MSG_H_
#define _SYS_MSG_H_

#include "context.h"

unsigned char set_extern_id(unsigned short addr, unsigned short newid);
unsigned char get_extern_module_type(unsigned short addr, unsigned short *module_type);



#endif /* _SYS_MSG_H_ */
