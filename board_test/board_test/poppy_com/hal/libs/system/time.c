/*
 * time.c
 *
 * Created: 30/07/2016 22:38:25
 *  Author: Xevel
 */ 
#include "asf.h"
#include "time.h"

/** Global g_ul_ms_ticks in milliseconds since start of application */
volatile uint32_t g_ul_ms_ticks = 0;

void SysTick_Handler(void)
{
    g_ul_ms_ticks++;
}

uint32_t get_tick(){
    return g_ul_ms_ticks;
}

void delay_ms(uint32_t ul_dly_ticks)
{
    uint32_t ul_cur_ticks;

    ul_cur_ticks = g_ul_ms_ticks;
    while ((g_ul_ms_ticks - ul_cur_ticks) < ul_dly_ticks);
}
