/*
 * poppyNetwork.c
 *
 * Created: 16/02/2015 11:53:28
 *  Author: nico
 *  Abstract: basics functionalities of the Poppy communication protocol
 */
#include "poppy-com/poppyNetwork.h"
#include "poppy-com/inc/i2c_master.h"
#include "poppy-com/inc/i2c_slave.h"
#include "poppy-com/inc/context.h"
#include HAL

//TO BE RM
// #include <avr/io.h>

extern context_t ctx;

// Startup and network configuration
void poppyNetwork_init(TX_CB tx_cb,
                       RX_CB rx_cb,
                       RX_CB rxgc_cb) {
// *************************This is bootload, no com_lib************************
    // /*
    //  * Set pin PB4 to 0.
    //  * This is used to detect if someone is plugged into the input.
    //  * Set pin PB5 to 0.
    //  * This is used to isolate the network input and output.
    //  */

    // /* Set pin 5 of PORTB in output mode. */
    // DDRB |= _BV(DDB5);
    // /* Set pin 4 of PORTB in output mode. */
    // DDRB |= _BV(DDB4);
    // /* Set pin 5 low to isolate input and output. */
    // PORTB &= ~_BV(PORTB5);
    // /* Set pin 4 low to pull SDA low. */
    // PORTB &= ~_BV(PORTB5);

    // /*
    //  * Wait for a while just to be sure that all other modules are started.
    //  * To do that we use timer 0.
    //  */
    // #if MAINCLOCK/100 <= 0xFF
    //     TCCR0B |= 0x01;
    //     unsigned int ticksCount = 255 - (MAINCLOCK/100);
    // #elif(MAINCLOCK/8)/1000 <= 0xFF
    //     TCCR0B |= 0x02;
    //     unsigned int ticksCount = 255 - ((MAINCLOCK/8)/100);
    // #elif(MAINCLOCK/64)/1000 <= 0xFF
    //     TCCR0B |= 0x03;
    //     unsigned int ticksCount = 255 - ((MAINCLOCK/64)/100);
    // #elif(MAINCLOCK/256)/1000 <= 0xFF
    //     TCCR0B |= 0x04;
    //     unsigned int ticksCount = 255 - ((MAINCLOCK/256)/100);
    // #elif(MAINCLOCK/1024)/1000 <= 0xFF
    //     TCCR0B |= 0x05;
    //     unsigned int ticksCount = 255 - ((MAINCLOCK/1024)/100);
    // #else
    //     #error *** Incompatible Clock set at MAINCLOCK. ***
    // #endif
    // /* Wait 10ms */
    // unsigned int i;
    // for (i = 0; i < 10; i++) {
    //     TCNT0 = ticksCount;
    //     while (TCNT0 >= ticksCount);
    // }

    // /*
    //  * At this point all modules should have put SDA low, we are able to see if someone is plugged into our input.
    //  */
    // /* Set SDA (pin 4 of PORTC) into input mode. */
    // DDRC &= ~_BV(DDC4);
    // /* Set SDA (pin 4 of PORTC) into tristated mode (hight-impedance). */
    // PORTC = PORTC & ~_BV(DDC4);


    // DDRC &= ~_BV(DDC5);
    // PORTC = PORTC & ~_BV(DDC5);

    // /* Read the value to know if a module is plugged before. */
    // if (!(PORTC & _BV(DDC4))) {
    //     /*
    //      * You are the first module of the robot!
    //      * You have to initiate the auto addressing routine.
    //      */

    //     /* Enable led just for debug! */
    //      // PORTB |= _BV(PORTB5);
    // } else {
    //     /*
    //      * You are not the first module of the robot!
    //      * Someone else will probably initiate the auto addressing routine.
    //      * You just have to wait your turn!
    //      */
    // }
// *****************************************************************************

    hal_init();

    // Save context
    // User side slave TX callback
    ctx.tx_cb = tx_cb;
    // User side slave RX callback
    ctx.rx_cb = rx_cb;
    // User side slave RX general call callback
    ctx.rxgc_cb = rxgc_cb;


    // Data callback
    ctx.data_cb = idle;

    // Module id
    ctx.id = DEFAULTID;
    // Module type
    ctx.type = MODULETYPE;


    // Status
    ctx.status = (status_t) {.rx_error = FALSE,
                             .master_write = FALSE,
                             .unexpected_state = FALSE,
                             .warning = FALSE};
}


unsigned char poppyNetwork_read(unsigned char addr, msg_t *msg) {
    i2cAddr(addr, TX);
    for (unsigned char i = 0; i < msg->size; i++) {
        msg->data[i] = i2cRead(1);
    }
    i2c_transmit(STOP);
    return 0;
    /*
     * TODO(NR) : dabord faire un WRITE puis un READ du coup msg contiens un
     *            truc a envoyer. La taille dans msg correspond donc au message
     *            en WRITE ensuite on dois spécifier le nombre d'octets a
     *            recevoir en READ et reremplir le msg.
    */
}

unsigned char poppyNetwork_write(unsigned char addr, msg_t *msg) {
    if (i2cAddr(addr, TX)) {
        i2c_transmit(STOP);
        return 1;
    }
    // Write DATA
    i2cWrite(msg->reg);
    i2cWrite(msg->size);
    for (unsigned char i = 0; i < msg->size; i++) {
        i2cWrite(msg->data[i]);
    }
    i2c_transmit(STOP);
    return 0;
}
