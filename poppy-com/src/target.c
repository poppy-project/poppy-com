#include "target.h"

// Global variables
context_t ctx;

unsigned char extra_target_bank(unsigned short val) {
    unsigned char i;
    for (i=0; i < ctx.max_extra_target; i++) {
        if (ctx.extra_target_bank[i] == val)
            return TRUE;
    }
    return FALSE;
}

void add_extra_target(unsigned short target) {
    ctx.extra_target_bank[ctx.max_extra_target++] = target;
}
