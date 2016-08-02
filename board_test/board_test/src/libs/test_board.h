/*
 * board.h
 *
 * Created: 31/07/2016 16:53:20
 *  Author: Xevel
 */ 


#ifndef BOARD_H_
#define BOARD_H_


//RS485
#define RS485_RX_PIN            IOPORT_CREATE_PIN(PIOB, 11)//
#define RS485_TX_PIN            IOPORT_CREATE_PIN(PIOB, 10)//
#define RS485_DE_PIN            IOPORT_CREATE_PIN(PIOA, 29)
#define RS485_RE_PIN            IOPORT_CREATE_PIN(PIOA, 24)

#define RS485_UART              USART4
#define RS485_UART_ID           ID_FLEXCOM4

#define RS485_UART_PORT         IOPORT_PIOB
#define RS485_UART_PINS         (PIO_PB11A_RXD4| PIO_PB10A_TXD4)
#define RS485_UART_PINS_FLAGS   (IOPORT_MODE_MUX_A)


// PTP_A
#define PTP_A_TX_PIN            IOPORT_CREATE_PIN(PIOA, 13)
#define PTP_A_RX_PIN            IOPORT_CREATE_PIN(PIOA, 12)
#define PTP_A_ADC_PIN           IOPORT_CREATE_PIN(PIOA, 17)
#define PTP_A_ADC_CHANNEL       ADC_CHANNEL_0

#define PTP_A_UART              USART5
#define PTP_A_UART_ID           ID_FLEXCOM5

#define PTP_A_UART_PORT         IOPORT_PIOA
#define PTP_A_UART_PINS         (PIO_PA13A_TXD5| PIO_PA12A_RXD5)
#define PTP_A_UART_PINS_FLAGS   (IOPORT_MODE_MUX_A)


// PTP_B
#define PTP_B_TX_PIN            IOPORT_CREATE_PIN(PIOA, 10)
#define PTP_B_RX_PIN            IOPORT_CREATE_PIN(PIOA, 9)
#define PTP_B_ADC_PIN           IOPORT_CREATE_PIN(PIOA, 18)
#define PTP_B_ADC_CHANNEL       ADC_CHANNEL_1

#define PTP_B_UART              USART0
#define PTP_B_UART_ID           ID_FLEXCOM0

#define PTP_B_UART_PORT         IOPORT_PIOA
#define PTP_B_UART_PINS         (PIO_PA10A_TXD0| PIO_PA9A_RXD0)
#define PTP_B_UART_PINS_FLAGS   (IOPORT_MODE_MUX_A)



#endif /* BOARD_H_ */