/*
 * button.c
 *
 * Created: 30/07/2016 22:41:24
 *  Author: Xevel
 */ 
#include "asf.h"


// #define IRQ_PRIOR_PIO    0
// 
// static void Button1_Handler(uint32_t id, uint32_t mask)
// {
//     if (PIN_PUSHBUTTON_1_ID == id && PIN_PUSHBUTTON_1_MASK == mask) {
// //         g_b_led0_active = !g_b_led0_active;
// //         if (!g_b_led0_active) {
//             ioport_set_pin_level(LED0_GPIO, IOPORT_PIN_LEVEL_HIGH);
// //        }
//     }
// }
// 
// 
// void configure_buttons(void)
// {
//     pmc_enable_periph_clk(PIN_PUSHBUTTON_1_ID);
//     pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
//     
//     pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID,
//     PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR, Button1_Handler);
//     NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
//     pio_handler_set_priority(PIN_PUSHBUTTON_1_PIO,
//     (IRQn_Type) PIN_PUSHBUTTON_1_ID, IRQ_PRIOR_PIO);
//     pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);
// }

