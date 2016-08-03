/*
 * ptp.h
 *
 * Created: 02/08/2016 15:27:59
 *  Author: Xevel
 */ 


#ifndef PTP_H_
#define PTP_H_
#include "asf.h"

typedef enum {
    PTP_MODE_HI_Z,
    PTP_MODE_TX,
    PTP_MODE_RX,
    PTP_MODE_RX_TX,
    PTP_MODE_HIGH,
    PTP_MODE_LOW,
    PTP_MODE_PULL_UP_WEAK,
    PTP_MODE_PULL_UP_STRONG,
    PTP_MODE_PULL_DOWN_WEAK,
    PTP_MODE_PULL_DOWN_STRONG
} ptp_mode_e;


typedef enum{
    PTP_HAL_UART,
    PTP_HAL_GPIO
} ptp_hal_mode_e;

typedef struct{
    ptp_mode_e          mode;
    ptp_hal_mode_e      hal_mode;
    usart_if            uart;
    ioport_port_t       uart_port;
    ioport_port_mask_t  uart_pin_mask;
    ioport_mode_t       tx_mode;
    ioport_mode_t       rx_mode;
    ioport_pin_t        tx_pin;
    ioport_pin_t        rx_pin;
    uint32_t            adc_channel;
} ptp_t;

extern ptp_t* ptp_a;
extern ptp_t* ptp_b;

void ptp_set_mode(ptp_t* ptp, ptp_mode_e mode);
void ptp_set_tx(ptp_t* ptp, bool enable);
void ptp_set_rx(ptp_t* ptp, bool enable);

uint32_t ptp_write(ptp_t* ptp, uint32_t c);
uint32_t ptp_read(ptp_t* ptp, uint32_t* c);

bool ptp_adc_get(ptp_t* ptp, uint32_t* adc_value);


bool ptp_hal_init(void);
bool ptp_init(void);


#endif /* PTP_H_ */