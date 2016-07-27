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
 * \enum msg_dir_t
 * \brief Message direction enum.
 *
 * This structure is used to get the message direction but it seems to be useles
 * because we have defferent interrupt for each msg_dir case.
 */
typedef enum {
    TX,   /*!< Slave transmiter mode. */
    RX,   /*!< Slave receiver mode. */
    RXGC, /*!< Slave receiver général call mode. */
    END   /*!< Slave receiver stop. */
}msg_dir_t;


/**
 * \struct msg_t
 * \brief Message structure.
 *
 * This structure is used to receive or send messages between modules in slave
 * and master mode.
 * please refer to the documentation
 */
typedef struct {
    unsigned int msg_type : 13;   /*!< Message type. */
    unsigned int ack_enable : 1;  /*!< TRUE = Enable ACK; FALSE = Disable ACK. */
    unsigned int size : 10;       /*!< Message size. */
    unsigned char data[512];      /*!< Data (512 bytes max). */
}msg_t;

typedef void (*RX_CB) (msg_dir_t dir, msg_t *msg);
typedef void (*TX_CB) (msg_t *msg);

/**
 * \fn void poppyNetwork_init(TX_CB tx_cb, RX_CB rx_cb, RX_CB rxgc_cb)
 * \brief Initialisation of the Poppy communication lib.
 *
 * \param tx_cb function pointer into the tx callback.
 * \param rx_cb function pointer into the rx callback.
 * \param rxgc_cb function pointer into the rx general call callback.
 *
 */
void poppyNetwork_init(TX_CB tx_cb,
                       RX_CB rx_cb,
                       RX_CB rxgc_cb);

/**
 * \fn unsigned char poppyNetwork_read(unsigned short addr, msg_t *msg)
 * \brief Master mode read function.
 *
 * \param addr Address of the slave.
 * \param msg Message to send to the slave, he come back with the reply of the slave.
 *
 */
unsigned char poppyNetwork_read(unsigned short addr, msg_t *msg,
                                unsigned short reply_size);

/**
 * \fn unsigned char poppyNetwork_write(unsigned short addr, msg_t *msg)
 * \brief Master mode write function.
 *
 * \param addr Address of the slave.
 * \param msg Message to send to the slave.
 */
unsigned char poppyNetwork_write(unsigned short addr, msg_t *msg);

/**
 **TODO**
 *
 *Faire la doc en commentaire
 *
 *Gérer le checksum
*/

#endif /* _POPPYNETWORK_H_ */
