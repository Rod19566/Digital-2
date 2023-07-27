#include "ADC_Interrupt.h"

// VARIABLES
/*
Variables
*/
#define _XTAL_FREQ 8000000


/*
 Configure ADC interrupt (optional):
 * •  Clear ADC interrupt flag 
 * •  Enable ADC interrupt
 * •  Enable peripheral interrupt
 * •  Enable global interrupt(1)
 */
void adc_init(int channel){
    ADCON0bits.ADCS = 0;            // select Fosc/2 (2us full TAD)
    ADCON0bits.CHS0 = 0;            // AN0
    ADCON0bits.ADON = 1;            // enable ADC
    ADCON1bits.VCFG1 = 0;           // reference voltages of PIC
    ADCON1bits.VCFG0 = 0;           // 0V to 5V
    ADCON1bits.ADFM = 0;            //    
    adc_change_channel(channel);
    
    PIR1bits.ADIF = 0;              //clean
    PIE1bits.ADIE = 1;               //ADC on
    ADCON0bits.GO = 1;               //conversion starts
    __delay_us(40);                 // Tiempo de adquisici n
    
}

unsigned char adc_read(unsigned char channel){
    adc_change_channel(channel);
    __delay_us(20);   //delay de 20 ms;
    return ADRESH;  
}

void adc_change_channel(unsigned char channel){    
    ADCON0bits.CHS = channel;    
}

int adc_get_channel(void){
    
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
        ADCON0bits.GO = 0;
        return 0;
}