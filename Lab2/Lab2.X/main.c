/*
 * File:   Lab2 LCD.c
 * Author: WMGWW
 *
 * Created on 20 de julio de 2023, 12:06 AM
 */

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

/*
Librerias
*/
#include <xc.h> // include processor files - each processor file is guarded.
#include <stdio.h> // include processor files - each processor file is guarded.
#include <stdint.h> // include processor files - each processor file is guarded.
#include "oscillator.h"
#include "setup.h"
#include "lcd.h"
#include "ADC_Interrupt.h"

// CONFIG1
#pragma config FOSC=INTRC_NOCLKOUT //Oscilador interno sin salida
#pragma config WDTE=OFF           //Reinicio repetitivo del pic
#pragma config PWRTE=OFF           //no espera de 72 ms al iniciar el pic
#pragma config MCLRE=OFF          //El pin MCLR se utiliza como entrada/salida
#pragma config CP=OFF             //Sin protección de código
#pragma config CPD=OFF            //Sin protección de datos
#pragma config BOREN=OFF //Sin reinicio cuando el input voltage es inferior a 4V
#pragma config IESO=OFF  //Reinicio sin cambio de reloj de interno a externo
#pragma config FCMEN=OFF //Cambio de reloj externo a interno en caso de fallas
#pragma config LVP=OFF    //Programación en low voltage apagada
 
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 8000000
/*
Prototipo de Funciones
*/

void LCD_Test(void);
/*
Variables
*/

unsigned char c = 0;                // ADC Value   
/*
Codigo actual
*/
void __interrupt() isr(void){
    
    if (ADIF == 1) {
        //ADC conversion
        c = adc_read(0);      //sets to ADC value 
        PIR1bits.ADIF = 0;              //ADC interrupt flag
        ADCON0bits.GO = 0;
    }
    
    //TMR0
     if (T0IF == 1) {      
        TMR0 = 0;
        INTCONbits.T0IF = 0; // TMR0 interrupt flag
     }   
    
}   
  

void main(void) {
    setupF(); 
    adc_init(1);
    Lcd_Init();      
    while(1){
        LCD_Test(); 
        if(ADCON0bits.GO == 0){   
            __delay_us(40);                 // Tiempo de adquisici n
            ADCON0bits.GO = 1;              // Iniciamos proceso de conversi n�
        }    
    }
    
}


void LCD_Test(){ 
    char buffer[12];
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Pot 1:     CPU:");
    Lcd_Set_Cursor(2,1);
    
    intToString(c, buffer);
    Lcd_Write_String(buffer);
    //Lcd_Set_Cursor(2,10);
    //Lcd_Write_String("Counter");
    __delay_ms(2000);
}
