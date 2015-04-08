#ifndef POPPY_COM_H_
#define POPPY_COM_H_

#include <avr/io.h>

#define MAINCLOCK 8000000

void poppyNetwork_init(void (*tx_cb)(volatile uint8_t *data),
                       void (*rx_cb)(volatile uint8_t data),
                       void (*rxgc_cb)(volatile uint8_t data));
uint8_t poppyNetwork_Read(uint8_t addr,
                          uint8_t *data,
                          uint8_t size);
uint8_t poppyNetwork_Write(uint8_t addr,
                           uint8_t *data,
                           uint8_t size);

/*
 **TODO**
 *
 *Dans le mode slave il doit y avoir un certain nombre de message qui seront réservé a l'utilisation
 *du protocole en lui même. C'est requettes la devrons être filtré pour être soit géré par le driver
 *lui même soit redirigé vers une CB spécifique.
 *La liste est a définir
 *
 *Les trucs relatif au boot doivent être virer, cette lib doit êttre utilisé tant du coté bootloader qu'utilisateur...
 *
 *Evaluer l'impact mémoire de la lib et limiter au maximum les structures de donnée lourde en interne, les déporter au niveau utilisateur.
 *
 *Rendre ce code compilable et flashable sur linux (faire un makefile propre)
 *
 *CODING STANDARDS (cpplint)
 *
 *Choir un compilo de doc (a priori Doxygen)
 *
 *Faire la doc en commentaire
 *
 *Tester le Read
 *
 *Tester le GC
 *
 *Monter clk
 *
 *Regarder comment fonctionne les lib arduino (ajout des trucs dans l'init gestion de clk et autre
 *
 *gitter tous ça!
*/

#endif /* POPPY_COM_H_ */
