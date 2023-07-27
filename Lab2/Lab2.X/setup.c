#include "setup.h"
#include "oscillator.h"
#include "lcd.h"
#include "ADC_Interrupt.h"

#define _XTAL_FREQ 8000000

void setupF(void){    
    configOsc(8);
    //Setting all I/O as digital pins
    ANSELH = 0;             //Digital Pins
    ANSELbits.ANS0  = 1;    //RA0 Analogic Pin
    ANSELbits.ANS1  = 1;//RA1 como pines analogicos
    TRISA  = 3;
    TRISC = 128;
    PORTA  = 0;             //clean PORTA

    
    TRISD = 0x00;
    PORTD  = 0;             //clean PORTD
      
    // TMR0
    OPTION_REGbits.T0CS = 0;        // internal clock (low to high)
    OPTION_REGbits.PSA  = 0;        // prescaler 
    OPTION_REGbits.PS = 0;          // 1:1

    ioc_init(1);
    
}
void ioc_init (char pin){
//configuracion interrupciones
    INTCONbits.GIE  = 1;        //se habilitan las interrupciones globales
    INTCONbits.PEIE = 1;        //se habilitan las interrupciones de los perifericos
//    INTCONbits.T0IE = 1;        //interrupcion overflow tmr0 habilitada
//
//    TMR0 = 0;
//    INTCONbits.T0IF = 0; // enable overflow interrupt TMR0
}


