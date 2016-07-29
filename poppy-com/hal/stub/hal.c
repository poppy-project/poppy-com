#include "hal.h"
#include "reception.h"

typedef enum {
    DATA_TX,
    ADDR_MATCH,
    DATA_RX,
    DATA_RXGC,
    STOP_CATCHED,
}status_reg_t;

// Global variables
extern context_t ctx;
unsigned char data_reg;
unsigned char data_size;
status_reg_t status_reg;
unsigned char gc = 0;


/**
 * \fn unsigned char hal_transmit(unsigned char* data)
 * \brief write a data byte
 *
 * \param data *data bytes to send
 * \param size size of data to send in byte
 *
 * \return error
 */
unsigned char hal_transmit(unsigned char* data, unsigned short size) {
   data_reg = *data;
   data_size = size;
    return 0;
}

void hal_init(void) {
    // I2C configuration
}

/**
 * \fn void id_update(unsigned char id)
 * \brief update the slave address
 *
 * \param id new address
 *
 */
void id_update(unsigned char id) {
    ctx.id = id;
    /*
     * TORO(NR) : Write this ID on EEPROM and use it as default at
     *            reboot (do this after debug)
    */
}

/**
 * \fn void alias_update(unsigned char id[16])
 * \brief update the slave address
 *
 * \param alias new address
 *
 */
void alias_update(unsigned char alias[16]) {
    for (unsigned char i=0; i < 16; i++)
        ctx.alias[i] = alias[i];
    // Write your alias on EEprom
}

