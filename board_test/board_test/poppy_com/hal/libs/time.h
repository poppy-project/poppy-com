/*
 * time.h
 *
 * Created: 31/07/2016 19:14:05
 *  Author: Xevel
 */ 


#ifndef TIME_H_
#define TIME_H_
#include "asf.h"

typedef struct {
    uint32_t start;
    uint32_t length;
} timeout_t;

uint32_t get_tick(void);
void delay_ms(uint32_t ul_dly_ticks);

void timeout_init(timeout_t* t, uint32_t length);
bool timeout_ended(timeout_t* t);
uint32_t timeout_elapsed(timeout_t* t);


#endif /* TIME_H_ */