/*
 * File:   Project1_Slave1.c
 * Author: WMGWW
 *
 * Created on 17 de agosto de 2023, 05:43 PM
 * Slave 1 
 * Sensor Ultrasónico
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
// DefiniciÃ³n e importaciÃ³n de librerÃ­as
//*****************************************************************************
#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include "I2C.h"
#include "ADC.h"
#include "oscillator.h"

//*****************************************************************************
// DefiniciÃ³n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
#define slave1Address 0x50

//Ultrasonic Sensor
#define TRIG RB2   //SALIDA 0
#define ECHO RB3   //ENTRADA 0
uint8_t z;
uint8_t receivedData;
uint16_t distance;

//*****************************************************************************
// DefiniciÃ³n de funciones para que se puedan colocar despuÃ©s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
uint16_t getDistance1();
void timer1Config(void);

//*****************************************************************************
// CÃ³digo de InterrupciÃ³n 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) { //reads data
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupciÃ³n recepciÃ³n/transmisiÃ³n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepciÃ³n se complete
            receivedData = SSPBUF;             // Saves buffer value in PORTD 
            //__delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){ //writes data
            z = SSPBUF;
            BF = 0;
            SSPBUF = distance;      //Sends value
            SSPCONbits.CKP = 1;
            //__delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

void main(void) {
    uint8_t counter = 0; // Inicializar el contador en 0    
    
    setup();
    timer1Config();  
    
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){     
        
      // adcValue = adcValue++;
      // PORTB = adcValue;
        
        if (receivedData == 1){
        //SENSOR ULTRASONICO 
        distance = getDistance1();    
        __delay_ms(200);
            
        }
       
    }
    return;
}


//*****************************************************************************
// FunciÃ³n de InicializaciÃ³n
//*****************************************************************************
void setup(void){
    configOsc(8);
    
    //////interrupciones habilitadas////////////    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Se habilitan las interrupciones perifericos
    
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0b00001000;
    TRISD = 0;
    
    PORTA = 0;         //se limpian los puertos
    PORTB = 0;
    PORTD = 0;
    I2C_Slave_Init(slave1Address);   
}


uint16_t getDistance1(){
    uint16_t Duration;
    uint16_t Distance;
    uint16_t Timer1;
    TRIG = 1;
    __delay_ms(10);
    TRIG = 0;
    while(ECHO == 0);
    T1CONbits.TMR1ON = 1;
    while(ECHO == 1);
    T1CONbits.TMR1ON = 0;
    Timer1 = TMR1;
    Duration = Timer1/2;
    if(Duration <= 23200)//400 cms de maximos del sensor si se usara float se usa el valor exacto 23529.4
    {
        Distance = Duration/58;
    }
    else if(Duration < 116)//rango minimo de 2 cm
        
    {
        Distance = 0;
    }
     else 
        
    {
        Distance = 0;
    }
    Duration = 0;
    TMR1 = 0;
    return Distance;
}


void timer1Config(void){
    //T1CONbits.RD16=1;
    T1CONbits.T1CKPS0 = 0b00;
    T1CONbits.TMR1CS = 0;
    TMR1 = 0;
    TMR1ON = 0;
}




