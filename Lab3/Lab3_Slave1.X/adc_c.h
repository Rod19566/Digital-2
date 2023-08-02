/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_ADC_C_H
#define	XC_ADC_C_H

#include <xc.h> // include processor files - each processor file is guarded.  
void adcConfig(void);
unsigned char adcChannel(unsigned char channel);
#endif	
