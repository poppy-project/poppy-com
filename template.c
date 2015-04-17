/*
 * template.c
 *
 * Created: 11/02/2015 12:40:48
 *  Author: Nicolas Rabault
 */

#include "poppy-com/poppyNetwork.h"

void rx_cb(msg_dir_t dir, msg_t *msg) {
    /*
     * Add your RX code here.
     */
}

void rxgc_cb(msg_dir_t dir, msg_t *msg) {
    /*
     * Add your RX general call code here.
     */
}

void tx_cb(msg_t *msg) {
    /*
     * Add your TX code here.
     */
}

int main(void) {
    poppyNetwork_init(tx_cb, rx_cb, rxgc_cb);

    /*
     * Add your main code here.
     */

    return 0;
}
