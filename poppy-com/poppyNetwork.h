/**
 * \file poppyNetwork.h
 * \brief Poppy communication main include file.
 * \author Nicolas Rabault
 * \version 0.1
 * \date 22 Avril 2015
 *
 * Include this file to use the poppy communication protocole.
 *
 */

#ifndef _POPPYNETWORK_H_
#define _POPPYNETWORK_H_

/**
 * \enum addr_mode_t
 * \brief Message addressing mode enum.
 *
 * This structure is used to get the message addressing mode list.
 */
typedef enum {
    ID,        /*!< Unique or virtual ID, used to send something to only one module. */
    IDACK,     /*!< Unique or virtual ID with reception Acknoledgment (ACK). */
    TYPE,      /*!< Type mode, used to send something to all module of the same type. */
    BROADCAST, /*!< Broadcast mode, used to send something to everybody. */
    MULTICAST  /*!< Multicast mode, used to send something to multiple modules. */
}target_mode_t;

/**
 * \struct msg_t
 * \brief Message structure.
 *
 * This structure is used to receive or send messages between modules in slave
 * and master mode.
 * please refer to the documentation
 */
typedef struct {
    union {
        struct {
            unsigned short protocol : 4;       /*!< Protocol version. */
            unsigned short target : 12;        /*!< Target address, it can be (ID, Multicast/Broadcast, Type). */
            unsigned short target_mode : 4;    /*!< Select targeting mode (ID, ID+ACK, Multicast/Broadcast, Type). */
            unsigned short source : 12;        /*!< Source address, it can be (ID, Multicast/Broadcast, Type). */
            unsigned char cmd;                 /*!< msg definition. */
            unsigned char size;                /*!< Size of the data field. */
        };
        unsigned char unmap[6];                /*!< Uncmaped form. */
    };
}header_t;

/**
 * \struct msg_t
 * \brief Message structure.
 *
 * This structure is used to receive or send messages between modules in slave
 * and master mode.
 * please refer to the documentation
 */
typedef struct {
    union {
        struct {
            header_t header;              /*!< Header filed. */
            unsigned char data[512];      /*!< Data with size known. */
        };
        unsigned char stream[512 + sizeof(header_t)]; /*!< unmaped option. */
    };
    union {
        unsigned short crc;
        volatile unsigned char ack;
    };
}msg_t;

typedef void (*RX_CB) (msg_t *msg);

/**
 * \fn void poppyNetwork_init(TX_CB tx_cb, RX_CB rx_cb, RX_CB rxgc_cb)
 * \brief Initialisation of the Poppy communication lib.
 *
 * \param rx_cb function pointer into the rx callback.
 *
 */
void poppyNetwork_init(RX_CB rxgc_cb);

/**
 * \fn unsigned char poppyNetwork_write(msg_t *msg)
 * \brief  Send message function.
 *
 * \param msg Message to send to the slave.
 */
unsigned char poppyNetwork_send(msg_t *msg);

#endif /* _POPPYNETWORK_H_ */
