/*
 * File:   uart.c
 * Author: WMGWW
 *
 * Created on 26 de julio de 2023, 07:27 AM
 */

#define _XTAL_FREQ 8000000

#include "uart.h"

void setupUart(void){
    
    //TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    UART_TX_config(1);
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    TXSTAbits.TXEN = 1;
}

void UART_TX_config (uint16_t baudrate){
    BAUDCTLbits.BRG16 = baudrate;
    SPBRG = 207;
    SPBRGH = 0;    
}

void UART_write_char(char c){
    while (TXSTAbits.TRMT == 0){
       
    }
    if (PIR1bits.TXIF){
            TXREG = c;
        }  
    
}

void write_UART(unsigned char data, unsigned char address){
    EEADR = address;
    EEDAT = data;
    
    EECON1bits.EEPGD = 0;   
    EECON1bits.WREN = 1;     
    
    INTCONbits.GIE = 0;      
    
    EECON2 = 0x55;           
    EECON2 = 0xAA;
    EECON1bits.WR = 1;      
    
    while(PIR2bits.EEIF==0);
    PIR2bits.EEIF = 0;
    
    INTCONbits.GIE = 1;     
    EECON1bits.WREN = 0;     

}


void UART_write_string(char* c){
    while (*c != 0){
      UART_write_char(*c);
      c++;
    }
    if (PIR1bits.TXIF){
            TXREG = 13;
        } 
    
}
unsigned char UART_read_char(unsigned char address){
    EEADR = address;    
    EECON1bits.EEPGD = 0;  
    EECON1bits.RD = 1;             // READS
    unsigned char  data = EEDAT;   // Save data in EEPROM
    //__delay_ms(20);
    return data;
    
    
}


