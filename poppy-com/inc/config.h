/*
 * This file contain default configuration of the project.
 */

#include "poppy-com/inc/mod_list.h"

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
#endif

#ifndef MAX_TRIES
    #define MAX_TRIES 5
#endif

#if MCU == atmega328p
    #define HAL "poppy-com/atmega328p/hal.h"
#endif
