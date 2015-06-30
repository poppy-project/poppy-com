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

#ifndef poppy_com_H_
#define poppy_com_H_

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
    RXGC  /*!< Slave receiver général call mode. */
}msg_dir_t;


/**
 * \struct msg_t
 * \brief Message structure.
 *
 * This structure is used to receive or send messages between modules in slave
 * and master mode.
 * please refer to ?? documentation
 */
typedef struct {
    unsigned char reg;        /*!< Message register. */
    unsigned char size;       /*!< Message size. */
    unsigned char data[512];  /*!< Data (512 bytes max). */
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
 * TODO :
 * - Return success or not.
 * - Check NULL pointers.
 */
void poppyNetwork_init(TX_CB tx_cb,
                       RX_CB rx_cb,
                       RX_CB rxgc_cb);

/**
 * \fn unsigned char poppyNetwork_read(unsigned char addr, msg_t *msg)
 * \brief Master mode read function.
 *
 * \param addr Address of the slave.
 * \param msg Message to send to the slave, he come back with the reply of the slave.
 *
 */
unsigned char poppyNetwork_read(unsigned char addr, msg_t *msg,
                                unsigned char reply_size);

/**
 * \fn unsigned char poppyNetwork_write(unsigned char addr, msg_t *msg)
 * \brief Master mode write function.
 *
 * \param addr Address of the slave.
 * \param msg Message to send to the slave.
 */
unsigned char poppyNetwork_write(unsigned char addr, msg_t *msg);

/**
 **TODO**
 *
 *Les trucs relatif au boot doivent être viré, cette lib doit êttre utilisé tant du coté bootloader qu'utilisateur...
 *
 *Evaluer l'impact mémoire de la lib et limiter au maximum les structures de donnée lourde en interne, les déporter au niveau utilisateur.
 *
 *Faire la doc en commentaire
 *
 *Monter clk
 *
 *Regarder comment fonctionne les lib arduino (ajout des trucs dans l'init gestion de clk et autre
 *
 *Gérer le checksum
*/

#endif /* poppy_com_H_ */
