/*
 * This file contain default configuration of the project.
 */

#include "poppy-com/inc/mod_list.h"

#define DEFAULTID 0x01

#ifndef MODULETYPE
    #define MODULETYPE DEV_BOARD
#endif

#ifndef MCU
    #define MCU "atmega328p"
#endif

#ifndef MAINCLOCK
    #define MAINCLOCK 8000000
#endif
