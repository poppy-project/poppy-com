/*
 * ptp.h
 *
 * Created: 27/07/2016 18:30:51
 *  Author: Xevel
 */ 


#ifndef PTP_H_
#define PTP_H_
#include "ptp_hal.h"


typedef enum{
    PTP_LOW,
    PTP_HIGH,
    PTP_PULLUP,
    PTP_PULLDOWN
} ptp_pin_state_t ;

extern ptp_t* ptp_upstream;
extern ptp_t* ptp_downstream;


bool ptp_digital_read( ptp_t* ptp );
bool ptp_detect_signal(void);

void ptp_set_input(void);
void ptp_reset_roles(void);
void ptp_set( ptp_t* ptp, ptp_pin_state_t state );


#endif /* PTP_H_ */