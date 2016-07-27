/*
 * tests.c
 *
 * Created: 11/02/2015 12:40:48
 *  Author: Nicolas Rabault
 */


#include "poppyNetwork.h"
#include "inc/sys_msg.h"
#include "test_mngmnt.h"

#include <stdio.h>

int test_value = 0;

typedef enum {
    TEST_CMD = PROTOCOL_CMD_NB,
    MODULE_REGISTER_NB
}module_register_t;

void rx_cb(msg_t *msg) {
    switch (msg->header.cmd) {
        case TEST_CMD :
            test_value = ((int)msg->data[0] << 8) | (int)msg->data[1];
        break;
        default :
            test_value = 0;
        break;
    }
}

/*
 * TEST SEQUENCES
 */

unsigned char test_init(void) {
    printf("\nInitialisation :\n");
    poppyNetwork_init(rx_cb);
    if (test(ctx.rx_cb == rx_cb)) return 1;
    return 0;
}


unsigned char set_id_brdcst(void) {
    printf("\nSet ID with BROADCAST mode :\n");
    if (test(!set_extern_id(BROADCAST, BROADCAST_VAL, 0x0A))) return 1;
    if (test(ctx.id == 0x0A)) return 1;
    return 0;
}

unsigned char set_id(void) {
    printf("\nSet ID in ID mode :\n");
    if (test(!set_extern_id(ID, ctx.id, 0x0B))) return 1;
    if (test(ctx.id == 0x0B)) return 1;
    return 0;
}

unsigned char set_id_type(void) {
    printf("\nSet ID in TYPE mode :\n");
    if (test(!set_extern_id(TYPE, ctx.type, 0x0C))) return 1;
    if (test(ctx.id == 0x0C)) return 1;
    return 0;
}

unsigned char set_id_ack(void) {
    // this one can't be tested in stub mode...
    printf("\nSet ID in IDACK mode :\n");
    if (test(!set_extern_id(IDACK, ctx.type, 0x0D))) return 1;
    if (test(ctx.id == 0x0D)) return 1;
    if (test(ctx.msg.ack)) return 1;
    return 0;
}

// unsigned char get_module(void) {
//     printf("\nGet extern module type\n");
//     unsigned char extrn_type;
//     if (test(!get_extern_module_type(0x0B, &extrn_type))) return 1;
//     if (test(extrn_type == DEV_BOARD)) return 1;
//     return 0;
// }

unsigned char write_id_mode(void) {
    printf("\nSend something in id_mode message :\n");
    msg_t msg = {.header.cmd = TEST_CMD,
                 .header.target = ctx.id,
                 .header.target_mode = ID,
                 .header.size = 2,
                 .data[0] = 0xCA,
                 .data[1] = 0xFE};

    if (test(!poppyNetwork_send(&msg))) return 1;
    if (test(test_value == 0xCAFE)) return 1;
    test_value = 0x0000;
    return 0;
}

unsigned char write_broadcast_mode(void) {
    printf("\nSend something in BROADCAST mode message :\n");
    msg_t msg = {.header.cmd = TEST_CMD,
                 .header.target = BROADCAST_VAL,
                 .header.target_mode = BROADCAST,
                 .header.size = 2,
                 .data[0] = 0xCA,
                 .data[1] = 0xFE};
    if (test(!poppyNetwork_send(&msg))) return 1;
    if (test(test_value == 0xCAFE)) return 1;
    test_value = 0x0000;
    return 0;
}

int main(void) {
    printf("test sequences :\n");

    test_sequences(test_init);
    test_sequences(set_id_brdcst);
    test_sequences(set_id);
    test_sequences(set_id_type);
    // test_sequences(get_module);
    test_sequences(write_id_mode);
    test_sequences(write_broadcast_mode);

    return test_end();
}
