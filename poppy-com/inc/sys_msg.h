#ifndef _SYS_MSG_H_
#define _SYS_MSG_H_

#include "context.h"

unsigned char set_extern_id(unsigned char addr, unsigned char newid);
unsigned char get_extern_module_type(unsigned char addr, unsigned char *module_type);



#endif /* _SYS_MSG_H_ */
