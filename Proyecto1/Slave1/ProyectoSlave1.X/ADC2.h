/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_ADC2_C_H
#define	XC_ADC2_C_H

#include <xc.h> // include processor files - each processor file is guarded.  
void adcConfig(void);
void adcChannel(unsigned char channel);
unsigned char adcRead();
#endif	
