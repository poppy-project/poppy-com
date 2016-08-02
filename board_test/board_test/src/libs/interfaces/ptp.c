/*
 * ptp.c
 *
 * Created: 31/07/2016 17:20:04
 *  Author: Xevel
 */ 
#include "asf.h"
#include "ptp.h"
#include "test_board.h"
#include "uart.h"


#define PTP_BAUDRATE 115200 // TODO get from somewhere else


//TODO see if there are glitches when going from one state to the other?
static void ptp_set_hal_mode(ptp_t* ptp, ptp_mode_e mode){
    if (ptp->mode != mode){
        switch (mode){
            case PTP_RX:
            case PTP_TX:
            case PTP_RX_TX:
                //TODO setup alternate function
            break;
            case PTP_HI_Z:
            case PTP_HIGH:
            case PTP_LOW:
            case PTP_PULL_UP_WEAK:
            case PTP_PULL_UP_STRONG:
            case PTP_PULL_DOWN_WEAK:
            case PTP_PULL_DOWN_STRONG:
                usart_disable_rx(ptp->uart);
                usart_disable_tx(ptp->uart);
                //TODO get back in GPIO mode HI-Z
            break;
        }
    }
}

void ptp_set_mode(ptp_t* ptp, ptp_mode_e mode){
    ptp_set_hal_mode(ptp, mode);

    switch(mode){
        case PTP_HI_Z:
            break;
        case PTP_RX:
            usart_disable_tx(ptp->uart);
            usart_enable_rx(ptp->uart);
            break;
        case PTP_TX:
            usart_disable_rx(ptp->uart);
            usart_enable_tx(ptp->uart);
            break;
        case PTP_RX_TX:
            usart_enable_rx(ptp->uart);
            usart_enable_tx(ptp->uart);
            break;
        case PTP_HIGH:
            ioport_set_pin_level(ptp->driver_pin, true);
            break;
        case PTP_LOW:
            ioport_set_pin_level(ptp->driver_pin, true);
            break;
        case PTP_PULL_UP_WEAK:
            ioport_set_pin_mode(ptp->driver_pin, IOPORT_MODE_PULLUP);  // TOOD check that it's the right way to do it, we want input with pull-up
            break;
        case PTP_PULL_UP_STRONG:
            //TODO // set pull up on all 3 pins
            break;
        case PTP_PULL_DOWN_WEAK:
            ioport_set_pin_mode(ptp->driver_pin, IOPORT_MODE_PULLDOWN); // TOOD check that it's the right way to do it, we want input with pull-down
            break;
        case PTP_PULL_DOWN_STRONG:
            //TODO // set pull up down all 3 pins
            break;
    }
    ptp->mode = mode;
}

void ptp_set_tx(usart_if uart, bool enable){
    if (enable){
        usart_enable_tx(uart);
    } else {
        usart_disable_tx(uart);
    }
}
void ptp_set_rx(usart_if uart, bool enable){
    if (enable){
        usart_enable_rx(uart);
    } else {
        usart_disable_rx(uart);
    }
}


uint32_t ptp_write(usart_if uart, uint32_t c){
    while (!usart_is_tx_ready(uart));
    return usart_write(uart, c);
}

uint32_t ptp_read(usart_if uart, uint32_t* c){
    return usart_read(uart, c);
}


bool ptp_hal_init(void){
    ioport_set_port_mode(PTP_A_UART_PORT, PTP_A_UART_PINS, PTP_A_UART_PINS_FLAGS);
    ioport_disable_port(PTP_A_UART_PORT, PTP_A_UART_PINS);
    uart_init(PTP_A_UART, PTP_BAUDRATE);
    
    // TODO ADC
    
    ioport_set_port_mode(PTP_B_UART_PORT, PTP_B_UART_PINS, PTP_B_UART_PINS_FLAGS);
    ioport_disable_port(PTP_B_UART_PORT, PTP_B_UART_PINS);
    uart_init(PTP_B_UART, PTP_BAUDRATE);
    
    return true;    
}

bool ptp_init(){
    return ptp_hal_init();    
}
