/*
 * uart.c
 *
 * Created: 30/07/2016 22:40:04
 *  Author: Xevel
 */ 
#include "asf.h"
#include "uart.h"
#include "conf_board.h"
#include "conf_clock.h"

static usart_serial_options_t uart_serial_options = {
    .charlength = US_MR_CHRL_8_BIT,
    .paritytype = US_MR_PAR_NO,
    .stopbits = US_MR_NBSTOP_1_BIT,
};

void uart_init(usart_if uart, uint32_t baudrate){
    uart_serial_options.baudrate = baudrate;
    stdio_serial_init(uart, &uart_serial_options);
}
