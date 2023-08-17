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



#include <xc.h>
#include <stdio.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000 // Oscillator frequency for __delay_ms()

void main(void) {
    // Initialization
    TRISC3 = 1; // SDA pin as input
    TRISC4 = 1; // SCL pin as input
    SSPSTAT = 0x80; // Slew rate control disabled
    SSPADD = 0x08; // Slave address
    SSPCON = 0x36; // I2C Slave mode, 7-bit address

    while (1) {
        while (!SSPIF); // Wait for I2C start condition
        SSPIF = 0; // Clear the flag
        if (!SSPOV && !WCOL) { // No overflow or collision
            if (SSPSTATbits.R_nW) { // Read operation
                // Implement your read code here
            } else { // Write operation
                // Implement your write code here
                while (!BF); // Wait for buffer to be full
                __delay_ms(10); // Add a delay for stability
            }
        }
        SSPCONbits.CKP = 1; // Release SCL line
    }
}
