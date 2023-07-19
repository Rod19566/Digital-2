#ifndef ADC_INTERRUPT_H
#define ADC_INTERRUPT_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

// PROTOTIPOS
void adc_init(int channel);
int adc_read ();
void adc_change_channel(int channel);
int adc_get_channel();

#endif //interrupt




