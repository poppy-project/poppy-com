/*
 * discovery.c
 *
 * Created: 03/08/2016 18:14:28
 *  Author: Xevel
 */ 
#include "discovery.h"
#include "time.h"
#include "ptp.h"
#include "pt.h"

#include "log.h"
#define LOG_TAG       "ptp"
#define LOG_LEVEL     LOG_LEVEL_DEBUG

ptp_t *ptp_upstream = NULL;
ptp_t *ptp_downstream = NULL;

static struct pt discovery_role_pt;
static
PT_THREAD(discovery_role_thread(struct pt *pt)){
    static ptp_t *observed_ptp, *other_ptp;
    static timeout_t timeout;
    
    PT_BEGIN(pt);
    observed_ptp = NULL;
    other_ptp = NULL;
    
    PT_WAIT_UNTIL(pt, ptp_digital_read(ptp_a) || ptp_digital_read(ptp_b));
    bool line_a_state = ptp_digital_read(ptp_a);
    bool line_b_state = ptp_digital_read(ptp_b);
    
    if (line_a_state && line_b_state ){
        // TODO what do we do if both are high?
        LOG_ERROR("Both lines high!");
    }
    if (!line_a_state && !line_b_state){
        LOG_WARNING("glitch");
        PT_RESTART(pt);
    }
    if (line_a_state){
        observed_ptp = ptp_a;
        other_ptp = ptp_b;
    } else {
        observed_ptp = ptp_b;
        other_ptp = ptp_a;
    }
    
    timeout_init(&timeout, 15); // TODO no magic value
    PT_WAIT_UNTIL(pt, ptp_digital_read(observed_ptp) == false
                    || timeout_ended(&timeout) );
    if( timeout_elapsed(&timeout) < 5 ) {
        LOG_WARNING("Too short!");
        PT_RESTART(pt);
    } else if (timeout_ended(&timeout)){
        LOG_ERROR("Timeout after rising edge");
        // TODO stop discovery altogether?
        PT_RESTART(pt);        
    }
    
    timeout_init(&timeout, 5); // TODO no magic value
    PT_WAIT_UNTIL(pt, ptp_digital_read(observed_ptp) == true
                    || timeout_ended(&timeout) );
    if (timeout_ended(&timeout)){
        // it's all good, we have seen the discovery pattern.
        ptp_upstream = observed_ptp;
        ptp_downstream = other_ptp;
        ptp_mode_set(ptp_upstream, PTP_MODE_HIGH);
        LOG_DEBUG("Discovery finished with %c as upstream",  ptp_upstream == ptp_a ? 'A' : 'B' );
    } else {
        LOG_ERROR("second rising edge");
        PT_RESTART(pt);
    }
    
    PT_END(pt);
}

bool discovery_run(){
    discovery_role_thread(&discovery_role_pt);
    if ( ptp_upstream != NULL ){ // TODO make a discovery status function ?
        return true;
    } else {
        return false;
    }
}

void ptp_reset_roles(){
    ptp_upstream = NULL;
    ptp_downstream = NULL;
}


void discovery_setup(){
    for (uint i = 0; i < 2; i++){
        ptp_t* ptp = ptp_lines[i];
        ptp_mode_set(ptp, PTP_MODE_PULL_DOWN_WEAK);
    }
    delay_ms(5);
    for (uint i = 0; i < 2; i++){
        ptp_t* ptp = ptp_lines[i];
        if( ptp_digital_read(ptp) ){
            ptp_error_set( ptp, PTP_ERROR_NOT_DISCOVERABLE);
        }
    }
}




