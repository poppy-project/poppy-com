#include "target.h"

// Global variables
context_t ctx;

unsigned char multicast_target_bank(unsigned short val) {
    unsigned char i;
    for (i=0; i < ctx.max_multicast_target; i++) {
        if (ctx.multicast_target_bank[i] == val)
            return TRUE;
    }
    return FALSE;
}

void add_multicast_target(unsigned short target) {
    ctx.multicast_target_bank[ctx.max_multicast_target++] = target;
}

unsigned char virtual_target_bank(unsigned short val) {
    unsigned char i;
    for (i=0; i < ctx.max_virtual_target; i++) {
        if (ctx.virtual_target_bank[i] == val)
            return TRUE;
    }
    return FALSE;
}

void add_virtual_target(unsigned short target) {
    ctx.virtual_target_bank[ctx.max_virtual_target++] = target;
}
