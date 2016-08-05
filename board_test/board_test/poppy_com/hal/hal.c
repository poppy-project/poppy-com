#include "hal.h"

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "uart.h"
#include "time.h"
#include "button.h"
#include "rs485.h"
#include "ptp.h"
#include "test_board.h"

#include "log.h"
#define LOG_TAG        "hak"
#define LOG_LEVEL     LOG_LEVEL_DEBUG


// Global variables
context_t ctx;

/**
 * \brief Interrupt handler for USART interrupt.
 */
void RS485_uart_irq_handler(void)
{
	UP1;
	uint32_t data;
	//LOG_INFO("plop");
	/* Get USART status and check if RXBUFF is set */
	if (usart_get_status(RS485_UART) & US_CSR_RXRDY) {
		usart_read(RS485_UART, &data);
		if (ctx.data_cb) {
			ctx.data_cb((volatile unsigned char*)&data);
		}
	}
	DW1;
}

/**
 * \fn void hal_init(void)
 * \brief hardware configuration (clock, communication, DMA...)
 */
void hal_init(void) {
    // init all things
	sysclk_init();
	board_init();

	uart_stdio_init(CONSOLE_UART, 115200);

	TEST_LOG_INFO(){
		printf("\n\r\tFirmware Poppy-com built on %s at %s\n\r", __DATE__, __TIME__ );
	}

	if (SysTick_Config(sysclk_get_cpu_hz() / 1000)) {
		LOG_ERROR("systic config failed");
		while (1);
	}

	rs485_init();
	ptp_init();
	    
	ptp_mode_set(ptp_a, PTP_MODE_RX);
	ptp_mode_set(ptp_b, PTP_MODE_RX);
}

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
	rs485_set_dir(HALF_DUPLEX_TX);
	for (unsigned short i = 0; i < size; i++)
	{
		if (rs485_write((uint32_t) data[i]))
			return 1;
	}
	while (!usart_is_tx_empty(RS485_UART));
	rs485_set_dir(HALF_DUPLEX_RX);
    return 0;
}

