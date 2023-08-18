/*
 * File:   I2CTester.c
 * Author: WMGWW
 *
 * Created on 17 de agosto de 2023, 12:12 AM
 */
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


//////////////////LIBRERIAS/////////////////
#include <xc.h>
#include <pic16f887.h>
#include <stdio.h>
#include <stdint.h>
#include "I2Clib.h"

/////////////////////VARIABLES/////////////////
#define _XTAL_FREQ 8000000 // Oscillator frequency for __delay_ms()

// Slave 1 I2C address (default address)
#define slave1_ADDRESS 0x50        //
#define slave2_ADDRESS 0x60        //
#define slave3_ADDRESS 0x70        //


char valueS1 = 0;          //dato pot s1 received ADC
char valueS2 = 0;          //dato pot s1 received ADC


////////////////// PROTOTIPOS ///////////////////
void setup(void);

////////////////////////////////////////////
void main(void) {
    setup();
    
    while(1){
        PORTA = valueS1;
        PORTB = valueS2;
         
        /////// COMUNICATION WITH SLAVE 1 
        //READ
        I2C_Master_Start();
        I2C_Master_Write(slave1_ADDRESS + 1);     //read
        valueS1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //WRITE      
        I2C_Master_Start();         //reads 
        I2C_Master_Write(slave1_ADDRESS);
        I2C_Master_Write(0b10000001);
        I2C_Master_Stop();
        __delay_ms(200);
        
        
        /////// COMUNICATION WITH SLAVE 2
        //READ
        I2C_Master_Start();
        I2C_Master_Write(slave2_ADDRESS + 1);     //read
        valueS2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //WRITE
        I2C_Master_Start();         //reads 
        I2C_Master_Write(slave2_ADDRESS);
        I2C_Master_Write(0b00011000);
        I2C_Master_Stop();
        __delay_ms(200);
        
    }
    
}

/////////////FUNCIONES///////////////
void setup(void){
        OSCCONbits.IRCF = 0b111;
        OSCCONbits.SCS = 1;
        
        ANSEL = 0;
        ANSELH = 0;

        TRISC = 128;
        TRISA = 0;
        TRISB = 0;

        PORTA = 0;
        PORTB = 0;
        PORTC = 0;
        I2C_Master_Init(100000);        // Inicializar ComuncaciÃ³n I2C

}