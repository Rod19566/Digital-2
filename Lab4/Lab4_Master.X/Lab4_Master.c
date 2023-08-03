/*
 * File:   Lab4_Master.c
 * Author: WMGWW
 *
 * Created on 3 de agosto de 2023, 12:34 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

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
#include <pic16f887.h>
#include <stdint.h>
#include <stdio.h>
#include "I2C.h"
#include "lcd.h"
#include "oscillator.h"

//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

// DS3232 I2C address (default address)
#define DS3232_ADDRESS 0x68         //     11010000   0x68

// Register addresses for time in DS3232
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02

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
char lineLCD[16];           //voltage chain
char s1ADC = 0;          //dato pot s1 received ADC
char counter = 0;          //dato pot s1 received ADC
char dateRTC[3] = {0,0,0};           //dato Date
char timeRTC[3] = {0,0,0};        //dato Time


#define upButton RB0
#define downButton RB1
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void) {
    
    if(RBIF == 1)  {        //interrupt due to change in button state
    if (!upButton){
        counter++;
        
    }
    if (!downButton){
        counter--;        
    }
        INTCONbits.RBIF = 0;
    }
}

//*****************************************************************************
// Main
//*****************************************************************************

void main(void) {
    setup();
    while(1){
        //PORTA = counter;
        
//        I2C_Master_Start();         //reads 
//        I2C_Master_Write(0x50);
//        I2C_Master_Write(counter);
//       // I2C_Master_Stop();
//        __delay_ms(200);
        
        //RTC  DS3232
        I2C_Master_Start();         //reads 
        I2C_Master_Write(DS3232_ADDRESS);        //followed by the direction bit (R/W),which  is  0  for  a  writ
        I2C_Master_Write(0x00);
        PORTA = I2C_Master_Read(0);
        timeRTC[0] = PORTA;
        //I2C_Master_RepeatedStart();
//        I2C_Master_Write(0x01);
//        timeRTC[1] = I2C_Master_Read(0);
//        I2C_Master_Write(0x02);
//        timeRTC[2] = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
       
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        s1ADC = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //PORTB++;   
        
        PORTA = timeRTC[0];
        //1101000 RTC ADDRESS
        
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);  //line 1
        sprintf(lineLCD, "S1:");
        Lcd_Write_String(lineLCD); 
        Lcd_Set_Cursor(1,8);  //line 1
        sprintf(lineLCD, "%.2d/%.2d/%.2d", dateRTC[0], dateRTC[1], dateRTC[2]);
        Lcd_Write_String(lineLCD); 
        
        Lcd_Set_Cursor(2,1);  //line 2
        sprintf(lineLCD,"%.2d", s1ADC);
        Lcd_Write_String(lineLCD);
        Lcd_Set_Cursor(2,8);  //line 2
        sprintf(lineLCD,"%.2d:%.2d:%.2d", timeRTC[0], timeRTC[1], timeRTC[2]);
        Lcd_Write_String(lineLCD); 
        
        __delay_ms(50);
    }
    return;
}


//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    configOsc(8);
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    //Setting PORTb as an input
    TRISB = 3;       //RB0 RB1 as inputs
    TRISC = 128;
    TRISD = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    Lcd_Init();
        
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Se habilitan las interrupciones perifericos
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE  = 1;

    // interrupt on change PORTB
    IOCBbits.IOCB0 = 1; //enable interrupt RB0, RB2
    IOCBbits.IOCB1 = 1;

    OPTION_REGbits.nRBPU = 0; //enable pullups
    //enable pullups RB0, RB2
    WPUBbits.WPUB0 = 1;      
    WPUBbits.WPUB1 = 1;
}

