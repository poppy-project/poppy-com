/*
 * This file contain default configuration of the project.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "src/mod_list.h"

#define DEFAULTID 0x01

#define DEFAULTHARDWAREMODE I2C

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

#if MCU == atmega328p
    #define HAL "extras/hal/atmega328p/hal.h"
#elif MCU == stub
    #define HAL "extras/hal/stub/hal.h"
#elif MCU== atmega64
    #define HAL "extras/hal/atmega64/hal.h"
#endif

#endif /* CONFIG_H_ */
