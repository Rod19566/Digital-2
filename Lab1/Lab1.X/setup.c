#include "setup.h"

#define _XTAL_FREQ 8000000

void setupF(void){    
    //Setting all I/O as digital pins
    ANSEL = 0;
    ANSELH = 0b00000000;       //
    
    //Setting PORTC as an output
    TRISC = 0x00;
    PORTC = 0; 
    //Setting PORTb as an input
    TRISB = 0b00000101;       //RB0 RB2 as inputs
    PORTB = 0; 
      
    // TMR0
    OPTION_REGbits.T0CS = 0;        // internal clock (low to high)
    OPTION_REGbits.PSA  = 0;        // prescaler 
    OPTION_REGbits.PS = 0;          // 1:1
    
    TMR0 = 0;
    INTCONbits.T0IF = 0; // enable overflow interrupt TMR0
    
    ioc_init(1);    //enable interrupt on change and pullup
    ioc_init(3);    //enable interrupt on change and pullup
}
void ioc_init (char pin){
           // PIR1bits.ADIF   = 0;
           // PIE1bits.ADIE   = 1;
           // INTCONbits.PEIE = 1;
            INTCONbits.GIE  = 1;
            INTCONbits.RBIF = 0;
            INTCONbits.RBIE  = 1;
            
            // interrupt on change PORTB
            IOCBbits.IOCB0 = 1; //enable interrupt RB0, RB2
            IOCBbits.IOCB2 = 1;
            
            OPTION_REGbits.nRBPU = 0; //enable pullups
            //enable pullups RB0, RB2
            WPUBbits.WPUB0 = 1;      
            WPUBbits.WPUB1 = 1;
}


void upButtonF(void){
        PORTC++;       
        
}

void downButtonF(void){
        PORTC--;    
}
