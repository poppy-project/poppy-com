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


void ptp_set_dir(usart_if uart, half_duplex_dir_e dir){
    switch(dir){
        case HALF_DUPLEX_NONE:
            usart_disable_rx(uart);
            usart_disable_tx(uart);
        break;
        case HALF_DUPLEX_RX:
            usart_disable_tx(uart);
            usart_enable_rx(uart);
        break;
        case HALF_DUPLEX_TX:
            usart_disable_rx(uart);
            usart_enable_tx(uart);
        break;
        case HALF_DUPLEX_BOTH:
            usart_enable_rx(uart);
            usart_enable_tx(uart);
        break;
    }
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
