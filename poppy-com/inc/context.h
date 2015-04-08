#ifndef CONTEXT_H_
#define CONTEXT_H_

typedef void (*RX_CB) (volatile uint8_t data);
typedef void (*TX_CB) (volatile uint8_t *data);

typedef struct {
        // Slave TX callback
        TX_CB tx_cb;
        // Slave RX callback
        RX_CB rx_cb;
        // Slave RX general call callback
        RX_CB rxgc_cb;
        // Module id
    }context;

context ctx;

#endif /* CONTEXT_H_ */
