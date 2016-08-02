#include "ptp_hal.h"
/*
* ptp.c
*
* Created: 27/07/2016 17:11:56
*  Author: Xevel
*/
#include "common.h"
#include "ptp.h"
#include "ptp_hal.h"

ptp_t ptp_lines[2] = { {(uint32_t)PIOA, PIO_PA2}, {(uint32_t)PIOA, PIO_PA3} }; // TODO

ptp_t *ptp_upstream = NULL;
ptp_t *ptp_downstream = NULL;


bool ptp_digital_read( ptp_t* ptp ){
    // TODO read digital state of PTP line
    return false;
}

bool ptp_detect_signal(){
    if ( ptp_digital_read(&ptp_lines[0]) ){
        ptp_upstream = &ptp_lines[0];
        ptp_downstream = &ptp_lines[1];
        return true;
    } else if ( ptp_digital_read(&ptp_lines[1]) ){
        ptp_upstream = &ptp_lines[1];
        ptp_downstream = &ptp_lines[0];
        return true;
    }
    return false;
}

void ptp_set_input(){
    // TODO make PTP lines both input with pull down
}

void ptp_reset_roles(){
    ptp_upstream = NULL;
    ptp_downstream = NULL;
}


void ptp_set(ptp_t* ptp, ptp_pin_state_t state ){
    switch(state){
        case PTP_HIGH:
            //TODO
        break;
        case PTP_LOW:
            //TODO
        break;
        case PTP_PULLUP:
            //TODO
        break;
        case PTP_PULLDOWN:
            //TODO
        break;
    }
}
