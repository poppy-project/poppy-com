#ifndef HALBOOT_H_
#define HALBOOT_H_

#include "poppy-com/inc/context.h"

typedef enum {
    // No pin are enabled
    NO_PIN_ENABLE,
    // The left pin already enabled
    LEFT_ENABLE,
    // The right pin already enabled
    RIGHT_ENABLE,
    // input and output pin are already enabled
    BOTH_ENABLE
}pin_state_t;

void halboot_init(void);
unsigned char pin(void);
void pass_token(void);

#endif /* HALBOOT_H_ */
