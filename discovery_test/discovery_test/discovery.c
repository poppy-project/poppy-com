/*
 * discovery.c
 *
 * Created: 27/07/2016 17:19:19
 *  Author: Xevel
 */ 
#include "common.h"
#include "ptp.h"
#include "ptp_hal.h"
#include "discovery.h"


uint16_t id = 0;

void id_reset(void){
    id = 0;
}

bool id_affected(void){
    return id != 0;
}


bool id_get(void){
    id_reset();
     
    while (!id_affected()){
        ptp_set_input();
        ptp_reset_roles();
        if (ptp_detect_signal()){
            ptp_set(ptp_upstream, PTP_PULLUP);
             
            // TODO talk with gate to receive an ID, declare name and type...
             
            // TODO detect downstream
             
            ptp_set(ptp_upstream, PTP_PULLDOWN);
        }
    }

    return id_affected();
}