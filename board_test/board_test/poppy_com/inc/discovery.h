/*
 * discovery.h
 *
 * Created: 03/08/2016 18:19:04
 *  Author: Xevel
 */ 


#ifndef DISCOVERY_H_
#define DISCOVERY_H_
#include "ptp.h"


extern ptp_t *ptp_upstream;
extern ptp_t *ptp_downstream;

void ptp_reset_roles(void);

bool discovery_run(void);

void discovery_setup(void);


#endif /* DISCOVERY_H_ */