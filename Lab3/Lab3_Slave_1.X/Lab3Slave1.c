/*
 * File:   Lab3Slave1.c
 * Author: WMGWW
 *
 * Created on 27 de julio de 2023, 04:16 PM
 */
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
#include "SPI.h"
#include "adc_c.h"
#include "oscillator.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

uint8_t temporal = 0;
unsigned char adcValue = 3;      //valor adc    
unsigned char slaveSelect = 0;

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void) {
    if (SSPIF == 1) {
        slaveSelect = spiRead();
        if (slaveSelect == 0)
            spiWrite(PORTB);

        SSPIF = 0; // Clear the SPI interrupt flag
    }

    if (ADIF == 1) {
        if (ADCON0bits.CHS == 0) { // Check if the ADC channel is AN0
            adcValue = adcRead();
            PIR1bits.ADIF = 0; // Clear the ADC interrupt flag
        }
    }
}


//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){          
       if (ADCON0bits.CHS == 0){
       adcChannel(0);     //se actualiza la variable con valor del adc
       //adcValue = adcRead();
        __delay_us(20);   //delay de 20 ms
        //PIR1bits.ADIF = 0;//se baja bandera interrupcion adc
        ADCON0bits.GO = 1;//inicio de la siguiente conversion
        }   
        PORTB = adcValue;
    }
    
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    configOsc(8);
    
    ANSELH = 0; //Pines digitales
    ANSELbits.ANS0  = 1;//RA0 como pines analogicos
    
    TRISA = 1;         //RA0 y RA1 como inputs
    TRISB = 0;
    
    PORTA = 0;         //se limpian los puertos
    PORTB = 0;
    
    adcConfig();
    __delay_us(40);
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}