#include "target.h"

unsigned char extra_target_bank(unsigned char val) {
    unsigned char i;
    for (i=0; i < max_extra_target; i++) {
        if (ctx.extra_target_bank[i] == val)
            return TRUE;
    }
    return FALSE;
}

void add_extra_target(unsigned char target) {
    ctx.extra_target_bank[ctx.max_extra_target++] = target;
}
