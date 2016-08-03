/*
 * This file contain default configuration of the project.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "mod_list.h"

#define DEFAULTID 0x01
#define PROTOCOL_REVISION 1
#define BROADCAST_VAL 0x0FFF

#ifndef MODULETYPE
    #define MODULETYPE DEV_BOARD
#endif

#ifndef MAINCLOCK
    #define MAINCLOCK 16000000
#endif

#ifndef SCLFREQ
    #define SCLFREQ 400000
#endif

#ifndef MAX_TRIES
    #define MAX_TRIES 5
#endif


#endif /* _CONFIG_H_ */
