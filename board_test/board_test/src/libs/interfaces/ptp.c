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

#include "time.h"

#define PTP_BAUDRATE 115200 // TODO get from somewhere else

ptp_t ptp_a_ = {
    .mode           = PTP_MODE_PULL_UP_STRONG,
    .uart           = PTP_A_UART,
    .uart_port      = PTP_A_UART_PORT,
    .uart_pin_mask  = PTP_A_UART_PIN_MASK,
    .tx_mode        = PTP_A_UART_TX_FLAGS,
    .rx_mode        = PTP_A_UART_RX_FLAGS,
    .tx_pin         = PTP_A_TX_PIN,
    .rx_pin         = PTP_A_RX_PIN,
    .adc_channel    = PTP_A_ADC_CHANNEL,
};
ptp_t ptp_b_ = {
    .mode           = PTP_MODE_PULL_UP_STRONG,
    .uart           = PTP_B_UART,
    .uart_port      = PTP_B_UART_PORT,
    .uart_pin_mask  = PTP_B_UART_PIN_MASK,
    .tx_mode        = PTP_B_UART_TX_FLAGS,
    .rx_mode        = PTP_B_UART_RX_FLAGS,
    .tx_pin         = PTP_B_TX_PIN,
    .rx_pin         = PTP_B_RX_PIN,
    .adc_channel    = PTP_B_ADC_CHANNEL,
};

ptp_t* ptp_a = &ptp_a_;
ptp_t* ptp_b = &ptp_b_;

static void ptp_set_hal_mode(ptp_t* ptp, ptp_hal_mode_e hal_mode ){
    if ( ptp->hal_mode != hal_mode ){
        switch(hal_mode){
            case PTP_HAL_UART:
                ioport_set_pin_mode(ptp->tx_pin, ptp->tx_mode);
                ioport_set_pin_mode(ptp->rx_pin, ptp->rx_mode);
                ioport_disable_port(ptp->uart_port,  ptp->uart_pin_mask);
            break;
            case PTP_HAL_GPIO:
                usart_disable_rx(ptp->uart);
                usart_disable_tx(ptp->uart);
            break;
        }
        ptp->hal_mode = hal_mode;
    }
}   


void ptp_set_mode(ptp_t* ptp, ptp_mode_e mode){
    ptp_set_hal_mode(ptp, mode);

    switch(mode){
        case PTP_MODE_RX:
            ptp_set_hal_mode(ptp, PTP_HAL_UART);
            usart_disable_tx(ptp->uart);
            usart_enable_rx(ptp->uart);
            break;
            
        case PTP_MODE_TX:
            ptp_set_hal_mode(ptp, PTP_HAL_UART);
            usart_disable_rx(ptp->uart);
            usart_enable_tx(ptp->uart);
            break;
            
        case PTP_MODE_RX_TX:
            ptp_set_hal_mode(ptp, PTP_HAL_UART);
            usart_enable_rx(ptp->uart);
            usart_enable_tx(ptp->uart);
            break;
            
        case PTP_MODE_HI_Z:
            ioport_set_port_mode(ptp->uart_port, ptp->uart_pin_mask, 0);
            ioport_set_port_dir(ptp->uart_port, ptp->uart_pin_mask, IOPORT_DIR_INPUT);
            ptp_set_hal_mode(ptp, PTP_HAL_GPIO);
            ioport_enable_port(ptp->uart_port,  ptp->uart_pin_mask);
            break;
            
        case PTP_MODE_HIGH:
            ioport_set_pin_level(ptp->tx_pin, true);
            ioport_set_pin_dir(ptp->tx_pin, IOPORT_DIR_OUTPUT);
            ptp_set_hal_mode(ptp, PTP_HAL_GPIO);
            ioport_enable_port(ptp->uart_port,  ptp->uart_pin_mask);
            break;
            
        case PTP_MODE_LOW:
            ioport_set_pin_level(ptp->tx_pin, false);
            ioport_set_pin_dir(ptp->tx_pin, IOPORT_DIR_OUTPUT);
            ptp_set_hal_mode(ptp, PTP_HAL_GPIO);
            ioport_enable_port(ptp->uart_port,  ptp->uart_pin_mask);
            break;
            
        case PTP_MODE_PULL_UP_WEAK:
            ioport_set_port_dir(ptp->uart_port, ptp->uart_pin_mask, IOPORT_DIR_INPUT);
            ptp_set_hal_mode(ptp, PTP_HAL_GPIO);
            ioport_set_pin_mode(ptp->tx_pin, IOPORT_MODE_PULLUP);
            ioport_set_pin_mode(ptp->rx_pin, 0);
            ioport_enable_port(ptp->uart_port,  ptp->uart_pin_mask);
            break;
            
        case PTP_MODE_PULL_UP_STRONG:
            ioport_set_port_dir(ptp->uart_port, ptp->uart_pin_mask, IOPORT_DIR_INPUT);
            ptp_set_hal_mode(ptp, PTP_HAL_GPIO);
            ioport_set_pin_mode(ptp->tx_pin, IOPORT_MODE_PULLUP);
            ioport_set_pin_mode(ptp->rx_pin, IOPORT_MODE_PULLUP); // TODO use ADC pull up too ?
            ioport_enable_port(ptp->uart_port,  ptp->uart_pin_mask);
            break;
            
        case PTP_MODE_PULL_DOWN_WEAK:
            ioport_set_port_dir(ptp->uart_port, ptp->uart_pin_mask, IOPORT_DIR_INPUT);
            ptp_set_hal_mode(ptp, PTP_HAL_GPIO);
            ioport_set_pin_mode(ptp->tx_pin, IOPORT_MODE_PULLDOWN);
            ioport_set_pin_mode(ptp->rx_pin, 0);
            ioport_enable_port(ptp->uart_port,  ptp->uart_pin_mask);
            break;
            
        case PTP_MODE_PULL_DOWN_STRONG:
            ioport_set_port_dir(ptp->uart_port, ptp->uart_pin_mask, IOPORT_DIR_INPUT);
            ptp_set_hal_mode(ptp, PTP_HAL_GPIO);
            ioport_set_pin_mode(ptp->tx_pin, IOPORT_MODE_PULLDOWN);
            ioport_set_pin_mode(ptp->rx_pin, IOPORT_MODE_PULLDOWN); // TODO use ADC pull up too ?
            ioport_enable_port(ptp->uart_port,  ptp->uart_pin_mask);
            break;
    }
    ptp->mode = mode;
}


void ptp_set_tx(ptp_t* ptp, bool enable){
    if (enable){
        usart_enable_tx(ptp->uart);
    } else {
        usart_disable_tx(ptp->uart);
    }
}
void ptp_set_rx(ptp_t* ptp, bool enable){
    if (enable){
        usart_enable_rx(ptp->uart);
    } else {
        usart_disable_rx(ptp->uart);
    }
}


uint32_t ptp_write(ptp_t* ptp, uint32_t c){
    while (!usart_is_tx_ready(ptp->uart));
    return usart_write(ptp->uart, c);
}

uint32_t ptp_read(ptp_t* ptp, uint32_t* c){
    return usart_read(ptp->uart, c);
}


bool ptp_adc_get(ptp_t* ptp, uint32_t* adc_value){
    adc_channel_disable(ADC, ADC_CHANNEL_ALL);
    adc_channel_enable(ADC, ptp->adc_channel);
    adc_start_software_conversion(ADC);
    delay_ms(1);    //TODO wait for conversion end?
    *adc_value = adc_channel_get_value(ADC, ptp->adc_channel);
    return true; // TODO
}


bool ptp_hal_init(void){
    
    // All pins are in pull-up at reset so turn that off, leave it on TX (driver_pin) only
    ioport_set_pin_mode(PTP_A_ADC_PIN, 0);
    ioport_set_pin_mode(PTP_A_RX_PIN, 0);

    ioport_set_pin_mode(PTP_B_ADC_PIN, 0);
    ioport_set_pin_mode(PTP_B_RX_PIN, 0);
    
    uart_init(PTP_A_UART, PTP_BAUDRATE);
    uart_init(PTP_B_UART, PTP_BAUDRATE);

    adc_enable();
    #if SAMG55
    adc_select_clock_source_mck(ADC);
    #endif
    struct adc_config config;
    adc_get_config_defaults(&config);
    adc_init(ADC, &config);
    adc_start_calibration(ADC);

    

//TODO check ADC setup compared to this, which comes from the ADC ENHANCED RESOLUTION example
//     	adc_enable();
//     	#if SAMG55
//     	adc_select_clock_source_mck(ADC);
//     	#endif
// 
//     	struct adc_config adc_cfg;
// 
//     	adc_get_config_defaults(&adc_cfg);
// 
//     	adc_init(ADC, &adc_cfg);
//     	adc_channel_enable(ADC, ADC_CHANNEL_1);
// 
//     	adc_set_trigger(ADC, ADC_TRIG_TIO_CH_0);
// 
//     	adc_set_callback(ADC, ADC_INTERRUPT_EOC_1,
//     	adc_end_conversion, 1);
// 
//     	set_adc_resolution();
//     	adc_start_calibration(ADC);
    return true;    
}

bool ptp_init(){
    return ptp_hal_init();    
}
