/*
 * This file contain default configuration of the project.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "mod_list.h"

#define DEFAULTID 0x01

#ifndef MODULETYPE
    #define MODULETYPE DEV_BOARD
#endif

#ifndef MCU
    #define MCU atmega328p
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
