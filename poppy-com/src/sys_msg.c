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

void send_ack(void) {
    unsigned char ack = TRUE;
    hal_transmit(&ack, 1);
}

unsigned char set_extern_id(target_mode_t target_mode, unsigned short target,
                            unsigned short newid) {
    msg_t msg;

    msg.header.target = target;
    msg.header.target_mode = target_mode;
    msg.header.cmd = WRITE_ID;
    msg.header.size = 2;
    msg.data[0] = newid;
    msg.data[1] = (newid <<8);

    if (!hal_transmit(msg.stream, msg.header.size + sizeof(header_t)))
        return 1;
    return 0;
}

// unsigned char get_extern_module_type(unsigned short addr,
//                                      unsigned short *module_type) {
//     if (hal_addr(addr, TX)) {
//         hal_transmit(STOP);
//         return 1;
//     }
//     if (hal_write(GET_MODULE_TYPE)) {
//         hal_transmit(STOP);
//         return 1;
//     }
//     if (hal_addr(addr, RX)) {
//         hal_transmit(STOP);
//         return 1;
//     }
//     if (hal_read(FALSE, module_type)) {
//         hal_transmit(STOP);
//         return 1;
//     }
//     hal_transmit(STOP);
//     return 0;
// }


 // // Reply with ID
 //    SET_ID,
 //    ping(unsigned short addr);
 //    // Reply with module_type number
 //    GET_MODULE_TYPE,
 //    // Reply with a status register
 //    GET_STATUS,
 //    // Reply with the actual firmware revision number
 //    GET_FIRM_REVISION,
