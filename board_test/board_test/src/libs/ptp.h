/*
 * ptp.h
 *
 * Created: 02/08/2016 15:27:59
 *  Author: Xevel
 */ 


#ifndef PTP_H_
#define PTP_H_

typedef enum {
    PTP_HI_Z,
    PTP_TX,
    PTP_RX,
    PTP_RX_TX,
    PTP_HIGH,
    PTP_LOW,
    PTP_PULL_UP_WEAK,
    PTP_PULL_UP_STRONG,
    PTP_PULL_DOWN_WEAK,
    PTP_PULL_DOWN_STRONG
} ptp_mode_e;



typedef struct{
    ptp_mode_e mode;
    usart_if uart;
    ioport_pin_t driver_pin;
    ioport_port_t driver_port;
    // TODO pins, adc...
} ptp_t;


void ptp_set_mode(ptp_t* ptp, ptp_mode_e mode);
void ptp_set_tx(usart_if uart, bool enable);
void ptp_set_rx(usart_if uart, bool enable);

uint32_t ptp_write(usart_if uart, uint32_t c);
uint32_t ptp_read(usart_if uart, uint32_t* c);

bool ptp_a_adc_get(uint32_t* adc);
bool ptp_b_adc_get(uint32_t* adc);


bool ptp_hal_init(void);
bool ptp_init(void);


#endif /* PTP_H_ */