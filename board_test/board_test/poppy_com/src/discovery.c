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

#define DISCOVERY_PULSE_HIGH_MAX_MS     (10)
#define DISCOVERY_PULSE_HIGH_MIN_MS     (6)

#define DISCOVERY_PULSE_HIGH_LEN_MS     ((DISCOVERY_PULSE_HIGH_MAX_MS + DISCOVERY_PULSE_HIGH_MIN_MS)/2)
#define DISCOVERY_PULSE_HIGH_PART_1_MS  (2)
#define DISCOVERY_PULSE_HIGH_PART_2_MS  (DISCOVERY_PULSE_HIGH_LEN_MS - DISCOVERY_PULSE_HIGH_PART_1_MS)

#define DISCOVERY_PULSE_LOW_MIN_MS      (6)
#define DISCOVERY_PULSE_LOW_MAX_MS      (12)

#define DISCOVERY_PULSE_LOW_LEN_MS      ((DISCOVERY_PULSE_LOW_MAX_MS + DISCOVERY_PULSE_LOW_MIN_MS)/2)
#define DISCOVERY_PULSE_LOW_PART_1_MS   (2)
#define DISCOVERY_PULSE_LOW_PART_2_MS   (2)
#define DISCOVERY_PULSE_LOW_PART_3_MS   (DISCOVERY_PULSE_HIGH_LEN_MS - DISCOVERY_PULSE_HIGH_PART_1_MS - DISCOVERY_PULSE_HIGH_PART_2_MS)


static struct pt discovery_upstream_pt;
static
PT_THREAD(discovery_upstream_thread(struct pt *pt)){
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
    
    timeout_init(&timeout, DISCOVERY_PULSE_HIGH_MAX_MS);
    PT_WAIT_UNTIL(pt, ptp_digital_read(observed_ptp) == false
                    || timeout_ended(&timeout) );
    if( timeout_elapsed(&timeout) < DISCOVERY_PULSE_HIGH_MIN_MS ) {
        LOG_WARNING("Too short!");
        PT_RESTART(pt);
    } else if (timeout_ended(&timeout)){
        LOG_ERROR("Timeout after rising edge");
        // TODO stop discovery altogether?
        PT_RESTART(pt);        
    }
    
    timeout_init(&timeout, DISCOVERY_PULSE_LOW_MIN_MS); 
    PT_WAIT_UNTIL(pt, ptp_digital_read(observed_ptp) == true
                    || timeout_ended(&timeout) );
    if (timeout_ended(&timeout)){
        // it's all good, we have seen the discovery pattern.
        ptp_upstream = observed_ptp;
        ptp_downstream = other_ptp;
        ptp_mode_set(ptp_upstream, PTP_MODE_HIGH);
        LOG_DEBUG("Discovery finished with %c as upstream",  ptp_upstream == ptp_a ? 'A' : 'B' );
        PT_EXIT(pt);
    } else {
        LOG_ERROR("second rising edge");
        PT_RESTART(pt);
    }
    
    PT_END(pt);
}

static struct pt discovery_downstream_pt;
static
PT_THREAD(discovery_downstream_thread(struct pt *pt)){
    static timeout_t timeout;
    
    PT_BEGIN(pt);

    // check the port is in the correct state (low and not marked undiscoverable)
    if ( ptp_status_flag_get(ptp_downstream, PTP_STATUS_DISCOVERY_FAILURE) ){
        LOG_ERROR("PTP downstream marked not discoverable");
        PT_EXIT(pt);
    }
    if (ptp_digital_read(ptp_downstream) == true){
        // TODO error it should be down, we can't detect stuff. mark this PTP as bad and move on...
        LOG_ERROR("PTP downstream not in discover mode (high instead of low)");
        ptp_status_flag_set(ptp_downstream, PTP_STATUS_DISCOVERY_FAILURE);
        PT_EXIT(pt);
    }
    
    // high pulse
    ptp_mode_set(ptp_downstream, PTP_MODE_HIGH);
    ptp_status_flag_set(ptp_downstream, PTP_STATUS_DISCOVERY_ONGOING);
    timeout_init(&timeout, DISCOVERY_PULSE_HIGH_PART_1_MS);
    PT_WAIT_UNTIL(pt, timeout_ended(&timeout));
    timeout_init(&timeout, DISCOVERY_PULSE_HIGH_PART_2_MS);
    PT_WAIT_UNTIL(pt, ptp_digital_read(ptp_downstream) == false
                        || timeout_ended(&timeout));
    if ( ! timeout_ended(&timeout) ){
        LOG_ERROR("Downstream pin down even though driven high");
        ptp_status_flag_set(ptp_downstream, PTP_STATUS_DISCOVERY_FAILURE);
        ptp_status_flag_clear(ptp_downstream, PTP_STATUS_DISCOVERY_ONGOING);
        PT_EXIT(pt);
    }
    
    // high pulse went out OK, put back low
    ptp_mode_set(ptp_downstream, PTP_MODE_PULL_DOWN_WEAK);
    timeout_init(&timeout, DISCOVERY_PULSE_LOW_PART_1_MS);
    PT_WAIT_UNTIL(pt, timeout_ended(&timeout));
    timeout_init(&timeout, DISCOVERY_PULSE_LOW_PART_2_MS);
    PT_WAIT_UNTIL(pt, ptp_digital_read(ptp_downstream) == true
                        || timeout_ended(&timeout));
    if ( ! timeout_ended(&timeout) ){
        LOG_ERROR("downstream answered too early");
        ptp_status_flag_set(ptp_downstream, PTP_STATUS_DISCOVERY_FAILURE);
        ptp_status_flag_clear(ptp_downstream, PTP_STATUS_DISCOVERY_ONGOING);
        PT_EXIT(pt);
    }        
    timeout_init(&timeout, DISCOVERY_PULSE_LOW_PART_3_MS);
    PT_WAIT_UNTIL(pt, ptp_digital_read(ptp_downstream) == true
                        || timeout_ended(&timeout));
    if ( timeout_ended(&timeout) ){
        // no response
        LOG_WARNING("downstream did not answered in time");
        ptp_status_flag_set(ptp_downstream, PTP_STATUS_EMPTY);
        ptp_status_flag_clear(ptp_downstream, PTP_STATUS_DISCOVERY_ONGOING);
        PT_EXIT(pt);
    }
    
    // low pulse OK and we have seen a response in time
    ptp_status_flag_set(ptp_downstream, PTP_STATUS_ENABLED);
    ptp_status_flag_clear(ptp_downstream, PTP_STATUS_DISCOVERY_FAILURE);
    ptp_status_flag_clear(ptp_downstream, PTP_STATUS_EMPTY);
    ptp_status_flag_set(ptp_downstream, PTP_STATUS_CONNECTED);
    
    PT_WAIT_UNTIL(pt, ptp_digital_read(ptp_downstream) == false );     
    // TODO do we need a timeout too ? the problem is that it would expire in the wrong order
    // (first node first instead of last like it would be preferable)
    
    // downstream finished discovery
    ptp_status_flag_clear(ptp_downstream, PTP_STATUS_DISCOVERY_ONGOING);
    
    PT_END(pt);
}    


bool discovery_run(){
    discovery_upstream_thread(&discovery_upstream_pt);
    discovery_downstream_thread(&discovery_downstream_pt);
    if ( ptp_upstream ){ // TODO make a discovery status function ?
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
            ptp_status_flag_set( ptp, PTP_STATUS_DISCOVERY_FAILURE);
        }
    }
}




