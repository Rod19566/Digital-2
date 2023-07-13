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

/*
Prototipo de Funciones
*/
void setup(void);
void upButton(void);
void downButton(void);



/*
Variables
*/
#define _XTAL_FREQ 8000000


/*
Codigo actual
*/


#include <xc.h>

void main(void) {
    setup();
    
    while(1){
        downButton();
        upButton();
        //__delay_ms(1000);
    }
    
}

void setup(void){
    configOsc(8);
    
    //Setting all I/O as digital pins
    ANSEL = 0;
    ANSELH = 0b00000000;
    
    //Setting PORTB as an output
    TRISB = 0x00;
    PORTB = 0;
    
    //Setting PORTA as an input
    TRISA   = 0b00000101;       //RA0 RA2 as inputs
}

void downButton(void){
    if(PORTAbits.RA2) {
        PORTB--;
        __delay_ms(400);
    }
}
void upButton(void){
    if(PORTAbits.RA0) {
        PORTB++;
        __delay_ms(400);
    }
}

