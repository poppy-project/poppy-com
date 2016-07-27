#include "target.h"

// Global variables
context_t ctx;

const int cmd_size[] = {
        0,    //GET_ID
        2,    //WRITE_ID
        0,    //GET_MODULE_TYPE
        0,    //GET_STATUS
        0,    //GET_FIRM_REVISION
        0     //GET_COM_REVISION
};

unsigned char extra_target_bank(unsigned char val) {
    unsigned char i;
    for (i=0; i < ctx.max_extra_target; i++) {
        if (ctx.extra_target_bank[i] == val)
            return TRUE;
    }
    return FALSE;
}

void add_extra_target(unsigned char target) {
    ctx.extra_target_bank[ctx.max_extra_target++] = target;
}
