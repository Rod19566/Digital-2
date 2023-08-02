/*
 * File:   Oscilador.c
 * Author: Dina Rodríguez
 *
 * Created on 26 de julio de 2021, 12:16 AM
 */
#include <stdint.h>
#include <xc.h>
#include "adc_c.h"

void adcConfig(void) {
    ADCON0bits.ADCS = 0;//00 se selecciona Fosc/2 para conversion (2us full TAD)
    ADCON0bits.CHS0 = 0;//se selecciona el canal AN0
    ADCON0bits.ADON = 1;//se enciende el adc
    ADCON1bits.VCFG1 = 0;//se ponen los voltajes de referencia internos del PIC
    ADCON1bits.VCFG0 = 0;//0V a 5V
    ADCON1bits.ADFM = 0; //se justifica a la izquierda, vals más significativos
    
    PIE1bits.ADIE = 1;          //se habilitan las interrupciones por adc
    INTCONbits.GIE  = 1; //se habilitan las interrupciones globales
    INTCONbits.RBIE = 1; //interrupcion on change habilitada
    INTCONbits.PEIE = 1; //se habilitan las interrupciones de los perifericos
    ADCON0bits.GO = 1;          //se comienza la conversion adc
    ADCON0bits.CHS = 0;
}

unsigned char adcChannel(unsigned char channel) {
    ADCON0bits.CHS = channel;
    return ADRESH;
}



