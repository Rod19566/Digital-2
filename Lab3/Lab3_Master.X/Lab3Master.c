/*
 * File:   Lab3.c
 * Author: WMGWW
 *
 * Created on 27 de julio de 2023, 03:05 PM
 */

//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "SPI.h"
#include "lcd.h"
#include "oscillator.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
#define RS RC0         
#define EN RC1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
char volt[16];            //voltage chain
char s1 = 0;             //dato pot S1
float s2 = 0;             //dato pot S2

char slaveSelect = 1;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup(); 
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){ 
       
        if (slaveSelect == 1){
            PORTCbits.RC2 = 0;       //Slave Select
            __delay_ms(1);

            spiWrite(slaveSelect);
            s1 = spiRead();

            __delay_ms(1);
            
            PORTCbits.RC2 = 1;       //Slave Deselect 
            __delay_ms(100);
            slaveSelect = 0;
       }
        else {
            PORTCbits.RC2 = 0;       //Slave Select
            __delay_ms(1);
//
            spiWrite(slaveSelect);
            s2 = spiRead();
            __delay_ms(1);
//
            PORTCbits.RC2 = 1;       //Slave Deselect 
            __delay_ms(100);
            slaveSelect = 1;
        }
       
       PORTB = s1;
       Lcd_Clear();
       Lcd_Set_Cursor(1,1);  //line 1
       Lcd_Write_String("Slave1 Pot:");
       Lcd_Set_Cursor(2,1);  
       sprintf(volt, "%.2d ", s1);
       Lcd_Write_String(volt);
       __delay_ms(500);       
       
    }
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    configOsc(8);
    ANSELH = 0; //Pines digitales
    TRISC = 144;
    TRISC2 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTCbits.RC2 = 1;
    INTCONbits.GIE  = 1; //se habilitan las interrupciones globales
    INTCONbits.PEIE = 1; //se habilitan las interrupciones de los perifericos
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    Lcd_Init();

}