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
#include "I2C.h"
#include "USART.h"
#include "LCD.h"

/////////////////////VARIABLES/////////////////
#define _XTAL_FREQ 8000000 // Oscillator frequency for __delay_ms()

// Slave 1 I2C address (default address)
#define slave1_ADDRESS 0x50        //
#define slave2_ADDRESS 0x60        //

#define mpu6050_ADDRESS     0x68       // WHEN ADO connected to GND
#define gyro_ADDRESS        0x43       // 

#define onOffButton RB0

#define RS RC0         
#define EN RC1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD

#define EN1 RA0
#define IN1 RA1
#define IN2 RA2
#define IN3 RA3
#define IN4 RA4
#define EN2 RA5


char ultrasonicValue = 0;
char fsrValue = 0;
char motor1 = 0;
char motor2 = 0;
int16_t gyroData[3];       // Array to store gyroscope data (X, Y, Z)
char lineLCD[16];           //voltage chain
int OnOff = 0;
char readFromESP = 0;
char toESP = 0;
char fanOnOff = 1;
int UARTcounter = 0;

////////////////// PROTOTIPOS ///////////////////
void setup(void);
void ReadGyroscopeData(int16_t *gyroData);
void slave1Communication(void);
void slave2Communication(void);
void LCDprint(void);
void usSensor(void);
void dcForward(void);
void dcRight(void);
void dcStop(void);
void ioc_init (char pin);


void __interrupt() isr(void){
    
    if(RBIF == 1)  {        //interrupt due to change in button state
    if (onOffButton == 1){
        if (OnOff == 1){
            OnOff = 0;
            fanOnOff = 0;
        }
        else OnOff = 1;        
    }
        INTCONbits.RBIF = 0;
    }
        
}

////////////////////////////////////////////
void main(void) {
    setup();
    UARTcounter = 1;
    
    while(1){
        //PORTB = motor1;
        
        if (OnOff == 1){
        /////// COMUNICATION WITH SLAVE 1 
        slave1Communication();
        /////// COMUNICATION WITH SLAVE 2
        slave2Communication();
        
        /////// COMUNICATION WITH Gyroscope
        //ReadGyroscopeData(gyroData); // Read gyroscope data   
            
        } else{
            
        }
        
        TXSTAbits.TXEN = 1; // Turn on UART transmission
        toESP = "Hello from the PIC";
        //UART
        if (UARTcounter == 1) {
            // toESP = "Power";
            // varToESP = OnOff;
            enviocadena("Power");  //new line in ASCII
            __delay_ms(300); // Delay before next reading  
            envioentero(OnOff);  //new line in ASCII
        } else if (UARTcounter == 2) {
            // toESP = "Fan";
            // varToESP = fanOnOff;
            enviocadena("Fan");  //new line in ASCII
            __delay_ms(300); // Delay before next reading  
            envioentero(fanOnOff);  //new line in ASCII
        } else if (UARTcounter == 3) {
            // toESP = "FSR";
            // varToESP = fsrValue;
            enviocadena("FSR");  //new line in ASCII 
            __delay_ms(300); // Delay before next reading  
            envioentero(fsrValue);  //new line in ASCII
        } else if (UARTcounter == 4) {
            // toESP = "Ultrasonic";
            // varToESP = ultrasonicValue;
            enviocadena("Ultrasonic");  //new line in ASCII
            __delay_ms(300); // Delay before next reading  
            envioentero(ultrasonicValue);  //new line in ASCII
        } else {
            enviocadena("None 404");  //new line in ASCII
            // toESP = "None";
            // varToESP = 404;
        }

        __delay_ms(300); // Delay before next reading  
        TXSTAbits.TXEN = 0; // Turn off UART transmission
        __delay_ms(300); // Delay before next reading  
        UARTcounter++;
        if (UARTcounter == 5) {
            UARTcounter = 1;
        } 
        //LCD
        LCDprint();
    } //loop
    
}

/////////////FUNCIONES///////////////
void setup(void){
        OSCCONbits.IRCF = 0b111;
        OSCCONbits.SCS = 1;
        
        //TX y RX para UART
        TXSTAbits.SYNC = 0;
        TXSTAbits.BRGH = 1;

        BAUDCTLbits.BRG16 = 1;
        
        SPBRG = 207; //baud 9600    
        SPBRGH = 0; 
        
        ANSEL = 0;
        ANSELH = 0;

        TRISC = 128;
        TRISA = 0;
        //Setting PORTb as an input
        TRISB = 1;       //RB0 as inputs
        TRISD = 0;
    
        PORTA = 0;
        PORTB = 0;
        PORTC = 0;
        PORTD = 0;
        
        RCSTAbits.SPEN = 1;
        RCSTAbits.RX9 = 0;
        RCSTAbits.CREN = 1;
        TXSTAbits.TXEN = 1;
        ioc_init(1);    //enable interrupt on change and pullup
        
        I2C_Master_Init(100000);        // Inicializar ComuncaciÃ³n I2C
        Lcd_Init();

}


        ////// // Function to read gyroscope data from MPU-6050
void ReadGyroscopeData(int16_t *gyroData) {
    I2C_Master_Start(); // Start I2C communication
    I2C_Master_Write(mpu6050_ADDRESS); // Send MPU-6050 address with write bit (0)
    I2C_Master_Write(0x43); // Register address of starting gyroscope data (X-axis high byte)
    
    I2C_Master_RepeatedStart(); // Restart I2C communication
    I2C_Master_Write(mpu6050_ADDRESS | 0x01); // Send MPU-6050 address with read bit (1)

    // Read gyroscope data
    gyroData[0] = (I2C_Master_Read(1) << 8) | I2C_Master_Read(1); // X-axis
    gyroData[1] = (I2C_Master_Read(1) << 8) | I2C_Master_Read(1); // Y-axis
    gyroData[2] = (I2C_Master_Read(1) << 8) | I2C_Master_Read(0); // Z-axis

    I2C_Master_Stop(); // Stop I2C communication//
    __delay_ms(200); // Delay before next reading
}

void slave1Communication(void){    
        char valueS1 = 0;          // received Data Slave 1
    //READ
        I2C_Master_Start();
        I2C_Master_Write(slave1_ADDRESS + 1);     //read
        valueS1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //WRITE      
        I2C_Master_Start();         //reads 
        I2C_Master_Write(slave1_ADDRESS);
        I2C_Master_Write(OnOff);
        I2C_Master_Stop();
        ultrasonicValue = valueS1;
        __delay_ms(200);
    
}

void slave2Communication(void){  
        char valueS2 = 0;          // received Data Slave 1    
        //READ
        I2C_Master_Start();
        I2C_Master_Write(slave2_ADDRESS + 1);     //read
        valueS2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //WRITE
        I2C_Master_Start();         //reads 
        I2C_Master_Write(slave2_ADDRESS);
        I2C_Master_Write(fanOnOff);
        I2C_Master_Stop();
        fsrValue = valueS2;
        __delay_ms(200);
        
    
}

void LCDprint(void){
    if (OnOff == 1){ 
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);  //line 1
        sprintf(lineLCD, "Distance: %.2d", ultrasonicValue);
        Lcd_Write_String(lineLCD); 
        usSensor();
        
        Lcd_Set_Cursor(2,1);  //line 2
        sprintf(lineLCD,"Basket: %.2d", fsrValue);
        Lcd_Write_String(lineLCD); 
            
        } else{
        //stops wheels
        //ultrasonicValue = 0;
        fsrValue = 0;
        dcStop();
        
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);  //line 1
        Lcd_Write_String("Off");
            
        }   
     
    
}


void usSensor(void){
    Lcd_Set_Cursor(1,15);  //line 1    
    
    if (ultrasonicValue <= 12) {
        Lcd_Write_String("!!"); 
        fanOnOff = 0;
        dcRight();
    }
    else  {
        Lcd_Write_String("  "); 
        fanOnOff = 1;
        dcForward();
    }
    
    
}

//void fsrSensor(void){
//    Lcd_Set_Cursor(2,9);  //line 1
//    
//    if (ultrasonicValue <= 100) {
//        Lcd_Write_String("!!"); 
//        dcRight();
//    }
//    else  {
//        Lcd_Write_String("  "); 
//        dcForward();
//    }
//    
//}


void dcForward(void){
    EN1 = 1;
    IN1 = 0;
    IN2 = 1;
    
    EN2 = 1;
    IN3 = 1;
    IN4 = 0;
    
}

void dcRight(void){
    EN1 = 1;
    IN1 = 0;
    IN2 = 1;
    
    EN2 = 1;
    IN3 = 0;
    IN4 = 1;
    
}

void dcStop(void){
    EN1 = 1;
    IN1 = 0;
    IN2 = 0;
    
    EN2 = 1;
    IN3 = 0;
    IN4 = 0;
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
            IOCBbits.IOCB1 = 1;
            
            OPTION_REGbits.nRBPU = 0; //enable pullups
            //enable pullups RB0, RB2
            WPUBbits.WPUB0 = 1;      
            WPUBbits.WPUB1 = 1;
}
