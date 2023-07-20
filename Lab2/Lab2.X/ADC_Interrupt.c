#include "ADC_Interrupt.h"

// VARIABLES
/*
Variables
*/
#define _XTAL_FREQ 8000000



void adc_init(int channel){
    ADCON0bits.ADCS = 0;            // select Fosc/2 (2us full TAD)
    ADCON0bits.CHS0 = 0;            // AN0
    ADCON0bits.ADON = 1;            // enable ADC
    ADCON1bits.VCFG1 = 0;           // reference voltages of PIC
    ADCON1bits.VCFG0 = 0;           // 0V to 5V
    ADCON1bits.ADFM = 0;            //
    
    PIE1bits.ADIE = 1;               //ADC on
    ADCON0bits.GO = 1;               //conversion starts
}

int adc_read (){
    return 0;    
}

unsigned char adc_change_channel(unsigned char channel){    
    ADCON0bits.CHS = channel;
    return ADRESH;
    
}

int adc_get_channel(){
    
        if(ADCON0bits.GO == 0){   
            if(ADCON0bits.CHS == 0b0000){
                ADCON0bits.CHS = 0b0001;    // Cambio de canal
            }
            
            else if(ADCON0bits.CHS == 0b0001){
                ADCON0bits.CHS = 0b0000;    // Cambio de canal  
            }
            __delay_us(40);                 // Tiempo de adquisici n
            ADCON0bits.GO = 1;              // Iniciamos proceso de conversi n�
        }
        ADCON0bits.GO == 0;
}