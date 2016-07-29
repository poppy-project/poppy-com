#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "config.h"
#include "poppyNetwork.h"
#include "cmd.h"

#define TRUE 1
#define FALSE 0

typedef void (*DATA_CB) (volatile unsigned char *data);

typedef struct {
    unsigned char rx_error : 1;
    unsigned char unexpected_state : 1;
    unsigned char warning : 1;
} status_t;


typedef struct {
    // Callback pointers
        DATA_CB data_cb;    /*!< Data management callback. */
        RX_CB rx_cb;        /*!< User side slave RX callback. */

    // Module infomations
        unsigned char id;       /*!< Module ID. */
        unsigned char type;     /*!< Module type. */
        unsigned char alias[16];/*!< Module alias. */

    // Variables
        status_t status;    /*!< Status. */
        msg_t msg;          /*!< Message. */
        unsigned char max_extra_target; /*!< Position pointer of the last extra target. */

        unsigned char extra_target_bank[256]; /*!< extra target bank. */
    }context_t;

context_t ctx;

#endif /* _CONTEXT_H_ */
