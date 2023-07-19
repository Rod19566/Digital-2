#include "ADC_Interrupt.h"

// VARIABLES
/*
Variables
*/
#define _XTAL_FREQ 8000000





void adc_init(int channel){
    
}

int adc_read (){
    return 0;    
}

void adc_change_channel(int channel){
    
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
    return 0;
}