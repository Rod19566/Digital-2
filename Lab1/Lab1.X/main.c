/*
 * File:   main.c
 * Author: WMGWW
 *
 * Created on 12 de julio de 2023, 04:20 PM
 */
/*   Set “Configuration Bits”
Palabras de configuración
*/
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

/*
Librerias
*/
#include <xc.h>
#include "oscillator.h"
#include "setup.h"
#include "ADC_Interrupt.h"
#include "sevensd.h"

/*
Prototipo de Funciones
*/

/*
Variables
*/
#define upButton RB0
#define downButton RB1
#define _XTAL_FREQ 8000000

unsigned char c = 0;                // ADC Value   
unsigned char display = 0;          //display

/*
Codigo actual
*/


#include <xc.h>


void __interrupt() isr(void){
    
    if(RBIF == 1)  {        //interrupt due to change in button state
    if (!upButton){
        upButtonF();
        
    }
    if (!downButton){
        downButtonF();
        
    }
        INTCONbits.RBIF = 0;
    }
    if (ADIF == 1) {
        //ADC comversion
        if (c == PORTC){
                  PORTBbits.RB4 = 1; //ALARM
              }
        else {
                  PORTBbits.RB4 = 0;
        }
        c = adc_change_channel(0);      //sets to ADC value
        __delay_us(20);                
        PIR1bits.ADIF = 0;              //ADC interrupt flag
        ADCON0bits.GO = 1;              //next conversion
    }
    
    //TMR0
     if (T0IF == 1) {
        TMR0 = 0;
        //display multiplexation
        switch(display) {
            case 0: 
                PORTBbits.RB2 = 0;
                PORTD = sevenSegmentDisplay(c & 0x0F);
                PORTBbits.RB3 = 1;
                display = 1; 
                break;
            case 1:
                PORTBbits.RB3 = 0;
                PORTD = sevenSegmentDisplay((c & 0xF0) >> 4);
                PORTBbits.RB2 = 1;
                display = 0;      
                break;
        }
    INTCONbits.T0IF = 0; // TMR0 interrupt flag
     }    
   return;         //INTERRUPT RETURN  
}

void main(void) {
    setupF();
    adc_init(1);
    
    while(1){
    }
    
}




