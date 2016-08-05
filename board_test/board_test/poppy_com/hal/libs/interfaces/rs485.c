/*
* rs485.c
*
* Created: 31/07/2016 17:19:54
*  Author: Xevel
*/
#include "asf.h"
#include "rs485.h"

#include "test_board.h"
#include "uart.h"

#include "log.h"
#define LOG_TAG       "rs485"
#define LOG_LEVEL     LOG_LEVEL_DEBUG


#define RS485_BAUDRATE 1000000 // TODO get it dynamically


void rs485_set_dir(half_duplex_dir_e dir){
    switch(dir){
        case HALF_DUPLEX_NONE:
		usart_disable_rx(RS485_UART);
        rs485_set_tx(false);
        rs485_set_rx(false);
        break;
        case HALF_DUPLEX_RX:
        rs485_set_tx(false);
        rs485_set_rx(true);
		usart_enable_rx(RS485_UART);
        break;
        case HALF_DUPLEX_TX:
		usart_disable_rx(RS485_UART);
        rs485_set_rx(false);
        rs485_set_tx(true);
        break;
        case HALF_DUPLEX_BOTH:
        rs485_set_tx(true);
        rs485_set_rx(true);
		usart_enable_rx(RS485_UART);
        break;
    }
}

void rs485_set_tx(bool enable){
    if (enable){
        ioport_set_pin_level(RS485_DE_PIN, IOPORT_PIN_LEVEL_HIGH);
    } else {
        ioport_set_pin_level(RS485_DE_PIN, IOPORT_PIN_LEVEL_LOW);
    }
}
void rs485_set_rx(bool enable){
    if (enable){
        ioport_set_pin_level(RS485_RE_PIN, IOPORT_PIN_LEVEL_LOW);
    } else {
        ioport_set_pin_level(RS485_RE_PIN, IOPORT_PIN_LEVEL_HIGH);
    }
}

bool rs485_hal_init(void){
    ioport_set_pin_dir(RS485_RE_PIN, IOPORT_DIR_OUTPUT);
    rs485_set_rx(false);

    ioport_set_pin_dir(RS485_DE_PIN, IOPORT_DIR_OUTPUT);
    rs485_set_tx(false);
	
	ioport_set_pin_sense_mode(RS485_RX_PIN, IOPORT_SENSE_RISING);
    
    ioport_set_port_mode(RS485_UART_PORT, RS485_UART_PIN_MASK, RS485_UART_PINS_FLAGS);
    ioport_disable_port(RS485_UART_PORT, RS485_UART_PIN_MASK);
    
    uart_init(RS485_UART, RS485_BAUDRATE);
	
    //delay_ms(10);
	
	/* Enable UART IRQ */
	usart_enable_interrupt(RS485_UART, US_IER_RXRDY);
	/* Enable UART interrupt */
	NVIC_EnableIRQ(RS485_UART_IRQn);
	
    rs485_set_dir(HALF_DUPLEX_RX);
    
    //TODO
    return true;
}


uint32_t rs485_write(uint32_t c){
	while (!usart_is_tx_ready(RS485_UART));
    uint32_t ret = usart_write(RS485_UART, c);
	return ret;
}

uint32_t rs485_read(uint32_t* c){
    return usart_read(RS485_UART, c);
}


bool rs485_init(void){
    rs485_hal_init();
    return true; // TODO
}

