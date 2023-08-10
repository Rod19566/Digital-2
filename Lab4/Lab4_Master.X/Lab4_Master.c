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
#define DS3232_ADDRESS 0b11010000        //     11010000   0x68

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
char counter = 0;          //dato contador
char mode = 0;          //dato mode editing seconds, minutes, or hours, days, month, year
char dateRTC[4] = {0,0,0,0};           //dato Date (dotw, date, month, year)

char timeRTC[3] = {0,0,0};        //dato Time (hour, min, sec)
unsigned char seconds, minutes, hours;


#define upButton RB0
#define downButton RB1
#define modeButton RB2
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
uint8_t BCD_to_Decimal(uint8_t bcd_value);
void settingRTC(char sec, char min, char hour, char day, char mon, char year);
uint8_t Decimal_to_BCD(uint8_t decimal_value);
void updateI2C(void);

//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void) {
    
    if(RBIF == 1)  {        //interrupt due to change in button state
    if (!upButton){
        switch(mode){
            case 0:
                settingRTC(timeRTC[2]+1,timeRTC[1],timeRTC[0],dateRTC[1],dateRTC[2],dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 1:
                settingRTC(timeRTC[2],timeRTC[1]+1,timeRTC[0],dateRTC[1],dateRTC[2],dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 2:
                settingRTC(timeRTC[2],timeRTC[1],timeRTC[0]+1,dateRTC[1],dateRTC[2],dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 3:
                settingRTC(timeRTC[2],timeRTC[1],timeRTC[0],dateRTC[1]+1,dateRTC[2],dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 4:
                settingRTC(timeRTC[2],timeRTC[1],timeRTC[0],dateRTC[1],dateRTC[2]+1,dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 5:
                settingRTC(timeRTC[2],timeRTC[1],timeRTC[0],dateRTC[1],dateRTC[2],dateRTC[3]+1);      //(char sec, char min, char hour, char day, char mon, char year)
                break;            
        }
        
    }
    if (!downButton){
        switch(mode){
            case 0:
                settingRTC(timeRTC[2]-1,timeRTC[1],timeRTC[0],dateRTC[1],dateRTC[2],dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 1:
                settingRTC(timeRTC[2],timeRTC[1]-1,timeRTC[0],dateRTC[1],dateRTC[2],dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 2:
                settingRTC(timeRTC[2],timeRTC[1],timeRTC[0]-1,dateRTC[1],dateRTC[2],dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 3:
                settingRTC(timeRTC[2],timeRTC[1],timeRTC[0],dateRTC[1]-1,dateRTC[2],dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 4:
                settingRTC(timeRTC[2],timeRTC[1],timeRTC[0],dateRTC[1],dateRTC[2]-1,dateRTC[3]);      //(char sec, char min, char hour, char day, char mon, char year)
                break;
            case 5:
                settingRTC(timeRTC[2],timeRTC[1],timeRTC[0],dateRTC[1],dateRTC[2],dateRTC[3]-1);      //(char sec, char min, char hour, char day, char mon, char year)
                break;            
        }      
    }
    if (!modeButton){
        mode++;        
    }
        INTCONbits.RBIF = 0;
    }
}

//*****************************************************************************
// Main
//*****************************************************************************

void main(void) {
    setup();
    settingRTC(12,12,12,12,12,23);
    while(1){        
//        I2C_Master_Start();         //reads 
//        I2C_Master_Write(0x50);
//        I2C_Master_Write(counter);
//       // I2C_Master_Stop();
//        __delay_ms(200);
        
        //RTC  DS3232
        I2C_Master_Start();         //reads 
        I2C_Master_Write(DS3232_ADDRESS);        //followed by the direction bit (R/W),which  is  0  for  a  writ
        I2C_Master_Write(SECONDS_REGISTER);
        // Restart communication with the DS3232
        I2C_Master_RepeatedStart();
        // Select DS3232 and read seconds
        I2C_Master_Write(DS3232_ADDRESS | 0x01); // Set read bit (LSB)
        // Read second
        timeRTC[2] = BCD_to_Decimal(I2C_Master_Read(1)); // Read seconds and send ACK
        timeRTC[1] = BCD_to_Decimal(I2C_Master_Read(1)); // Read minutes and send ACK
        timeRTC[0] = BCD_to_Decimal(I2C_Master_Read(1)); // Read hours and send ACK
        
        dateRTC[0] = BCD_to_Decimal(I2C_Master_Read(1)); // Read day and send ACK
        dateRTC[1] = BCD_to_Decimal(I2C_Master_Read(1)); // Read date and send ACK
        dateRTC[2] = BCD_to_Decimal(I2C_Master_Read(1)); // Read month and send ACK
        dateRTC[3] = BCD_to_Decimal(I2C_Master_Read(0)); // Read year and send ACK
        I2C_Master_Stop();
        __delay_ms(200);
        
        /////// COMUNICATION WITH SLAVE 1
        I2C_Master_Start();
        I2C_Master_Write(0x51);     //read
        s1ADC = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //PORTB++;   
        
        //PORTA = timeRTC[0];

        Lcd_Clear();
        Lcd_Set_Cursor(1,1);  //line 1
        sprintf(lineLCD, "S1:");
        Lcd_Write_String(lineLCD); 
        Lcd_Set_Cursor(1,6);  //line 1
        sprintf(lineLCD, "%.2d/%.2d/20%.2d", dateRTC[1], dateRTC[2], dateRTC[3]);
        Lcd_Write_String(lineLCD); 
        
        Lcd_Set_Cursor(2,1);  //line 2
        sprintf(lineLCD,"%.2d", s1ADC);
        Lcd_Write_String(lineLCD);
        Lcd_Set_Cursor(2,7);  //line 2
        sprintf(lineLCD,"%.2d:%.2d:%.2d", timeRTC[0], timeRTC[1], timeRTC[2]);
        Lcd_Write_String(lineLCD); 
        
        __delay_ms(50);
        if (mode == 6){
            mode = 0;
        }
        //PORTA = counter;
        PORTA = mode;
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
    TRISB = 0b00000111;       //RB0 RB1 RB2 as inputs
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
    IOCBbits.IOCB2 = 1;

    OPTION_REGbits.nRBPU = 0; //enable pullups
    //enable pullups RB0, RB2
    WPUBbits.WPUB0 = 1;      
    WPUBbits.WPUB1 = 1;   
    WPUBbits.WPUB2 = 1;
}

void settingRTC(char sec, char min, char hour, char day, char mon, char year){
            //RTC  DS3232
        I2C_Master_Start();         //reads 
        I2C_Master_Write(DS3232_ADDRESS);
        I2C_Master_Write(SECONDS_REGISTER);

        // Write seconds to DS3232
        I2C_Master_Write(Decimal_to_BCD(sec));

        // Write minutes to DS3232
        I2C_Master_Write(Decimal_to_BCD(min));

        // Write hours to DS3232
        I2C_Master_Write(Decimal_to_BCD(hour));
        // Write seconds to DS3232
        I2C_Master_Write(Decimal_to_BCD(day));//weekday
        I2C_Master_Write(Decimal_to_BCD(day));//date

        // Write minutes to DS3232
        I2C_Master_Write(Decimal_to_BCD(mon));

        // Write hours to DS3232
        I2C_Master_Write(Decimal_to_BCD(year));

        // Stop communication with the DS3232
        I2C_Master_Stop();
}

uint8_t BCD_to_Decimal(uint8_t bcd_value) {
    // Extract the tens digit from the BCD value
    uint8_t tens_digit = (bcd_value >> 4) & 0x0F;

    // Extract the ones digit from the BCD value
    uint8_t ones_digit = bcd_value & 0x0F;

    // Convert the tens and ones digits to decimal
    uint8_t decimal_value = (tens_digit * 10) + ones_digit;

    return decimal_value;
}

uint8_t Decimal_to_BCD(uint8_t decimal_value) {
    // Extract the tens and ones digits from the decimal value
    uint8_t tens_digit = decimal_value / 10;
    uint8_t ones_digit = decimal_value % 10;

    // Combine the tens and ones digits into a BCD value
    uint8_t bcd_value = (tens_digit << 4) | ones_digit;

    return bcd_value;
}

void updateI2C(){
    
}
