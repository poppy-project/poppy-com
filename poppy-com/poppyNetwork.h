#ifndef poppy_com_H_
#define poppy_com_H_

typedef enum {
    TX,
    RX,
    RXGC
}msg_dir_t;

typedef struct {
    // Message register
    uint8_t reg;
    // Message size
    uint8_t size;
    // Data
    uint8_t data[512];
}msg_t;

typedef void (*RX_CB) (msg_dir_t dir, msg_t *msg);
typedef void (*TX_CB) (msg_t *msg);

void poppyNetwork_init(TX_CB tx_cb,
                       RX_CB rx_cb,
                       RX_CB rxgc_cb);
uint8_t poppyNetwork_read(uint8_t addr,
                           msg_t *msg);
uint8_t poppyNetwork_write(uint8_t addr,
                           msg_t *msg);

/*
 **TODO**
 *
 *Les trucs relatif au boot doivent être viré, cette lib doit êttre utilisé tant du coté bootloader qu'utilisateur...
 *
 *Evaluer l'impact mémoire de la lib et limiter au maximum les structures de donnée lourde en interne, les déporter au niveau utilisateur.
 *
 *Choir un compilo de doc (a priori Doxygen)
 *
 *Faire la doc en commentaire
 *
 *Tester le GC
 *
 *Monter clk
 *
 *Regarder comment fonctionne les lib arduino (ajout des trucs dans l'init gestion de clk et autre
 *
 *Gérer les retry direct dans la lib.
 *
 *Gérer le checksum
*/

#endif /* poppy_com_H_ */
