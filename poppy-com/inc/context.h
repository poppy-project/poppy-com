#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "poppy-com/inc/config.h"
#include "poppy-com/poppyNetwork.h"

#define TRUE 1
#define FALSE 0

typedef void (*DATA_CB) (msg_dir_t dir, volatile unsigned char *data);

typedef struct {
  unsigned char rx_error : 1;
  unsigned char master_write : 1;
  unsigned char master_read : 1;
  unsigned char unexpected_state : 1;
  unsigned char warning : 1;
} status_t;

/*
 * Tous les messages coté applicatif devrons avoir une structure de type :
 * ADDR - REGISTER - SIZE - DATA[512] - CHECKSUM
 */
typedef enum {
    // Reply with ID
    GET_ID,
    // Get and save a new given ID
    WRITE_ID,
    // Reply with module_type number
    GET_MODULE_TYPE,
    // Reply with a status register
    GET_STATUS,
    // Reply with the actual firmware revision number
    GET_FIRM_REVISION,
    // Get a New firmware and write it in flash
    WRITE_FIRMWARE
}register_t;

typedef struct {
        // Data management callback.
        DATA_CB data_cb;
        // User side slave TX callback
        TX_CB tx_cb;
        // User side slave RX callback
        RX_CB rx_cb;
        // User side slave RX general call callback
        RX_CB rxgc_cb;

    // Module infomations
        // Module id
        unsigned char id;
        // Module type
        unsigned char type;

    // Variables
        // Status
        status_t status;
        // Message
        msg_t msg;
    }context_t;

context_t ctx;

#endif /* CONTEXT_H_ */
