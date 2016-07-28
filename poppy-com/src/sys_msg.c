#include "poppyNetwork.h"
#include "sys_msg.h"
#include "hal.h"

/* Specific system mesages :
 * These messages don't follow generic rules of this protocol, there are
 * protocols level messages.
 * Please use it with caution
 */

// Global variables
extern context_t ctx;

unsigned char set_extern_id(unsigned short addr, unsigned short newid) {
    if (hal_addr(addr, TX)) {
        hal_transmit(STOP);
        return 1;
    }
    if (hal_write(WRITE_ID)) {
        hal_transmit(STOP);
        return 1;
    }
    if (hal_write(newid)) {
        hal_transmit(STOP);
        return 1;
    }
    hal_write(crc(&newid, 1));
    hal_transmit(STOP);
    return 0;
}

unsigned char get_extern_module_type(unsigned short addr,
                                     unsigned short *module_type) {
    if (hal_addr(addr, TX)) {
        hal_transmit(STOP);
        return 1;
    }
    if (hal_write(GET_MODULE_TYPE)) {
        hal_transmit(STOP);
        return 1;
    }
    if (hal_addr(addr, RX)) {
        hal_transmit(STOP);
        return 1;
    }
    if (hal_read(FALSE, module_type)) {
        hal_transmit(STOP);
        return 1;
    }
    hal_transmit(STOP);
    return 0;
}


 // // Reply with ID
 //    SET_ID,
 //    ping(unsigned short addr);
 //    // Reply with module_type number
 //    GET_MODULE_TYPE,
 //    // Reply with a status register
 //    GET_STATUS,
 //    // Reply with the actual firmware revision number
 //    GET_FIRM_REVISION,
