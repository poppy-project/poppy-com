/*
 * test.c
 *
 * Created: 11/02/2015 12:40:48
 *  Author: Nicolas Rabault
 */


#include "poppyNetwork.h"
#include "i2c_master.h"
#include "test_mngmnt.h"

#include <stdio.h>

extern context_t ctx;

int test_value = 0;

typedef enum {
    TEST_REGISTER = PROTOCOL_REGISTER_NB,
    MODULE_REGISTER_NB
}module_register_t;

void rx_cb(msg_dir_t dir, msg_t *msg) {
    switch (msg->type) {
        case TEST_REGISTER :
            test_value = ((int)msg->data[0] << 8) | (int)msg->data[1];
        break;
        default :
            test_value = 0;
        break;
    }
}

void rxgc_cb(msg_dir_t dir, msg_t *msg) {
}

void tx_cb(msg_t *msg) {
}

void Init(void) {
    poppyNetwork_init(tx_cb, rx_cb, rxgc_cb);
}

/*
 * TEST SEQUENCES
 */

unsigned char test_init(void) {
    printf("\nInitialisation :\n");
    Init();
    if (test(ctx.tx_cb == tx_cb)) return 1;
    if (test(ctx.rx_cb == rx_cb)) return 1;
    if (test(ctx.rxgc_cb == rxgc_cb)) return 1;
    return 0;
}


unsigned char id_gc(void) {
    printf("\nWrite ID GC :\n");
    if (test(!set_extern_id(0x00, 0x0A))) return 1;
    if (test(ctx.id == 0x0A)) return 1;
    return 0;
}

unsigned char id(void) {
    printf("\nWrite ID :\n");
    if (test(!set_extern_id(0x01, 0x0B))) return 1;
    if (test(ctx.id == 0x0B)) return 1;
    return 0;
}

unsigned char get_module(void) {
    printf("\nGet extern module type\n");
    unsigned char extrn_type;
    if (test(!get_extern_module_type(0x0B, &extrn_type))) return 1;
    if (test(extrn_type == DEV_BOARD)) return 1;
    return 0;
}

unsigned char write(void) {
    printf("\nWrite message :\n");
    msg_t msg = {
      .type = TEST_REGISTER,
      .size = 2,
      .data[0] = 0xCA,
      .data[1] = 0xFE
    };
    if (test(!poppyNetwork_write(0x01, &msg))) return 1;
    if (test(test_value == 0xCAFE)) return 1;
    return 0;
}

int main(void) {
    printf("test sequences :\n");

    test_sequences(test_init);
    test_sequences(id_gc);
    test_sequences(id);
    test_sequences(get_module);
    test_sequences(write);

    return test_end();
}
