/*
 * File:   USART.c
 * Author: WMGWW
 *
 * Created on 17 de agosto de 2023, 09:33 AM
 */

#include "usart.h"

void write(unsigned char data, unsigned char address){
    EEADR = address;
    EEDAT = data;
    
    EECON1bits.EEPGD = 0;   // Escribir a memoria de datos
    EECON1bits.WREN = 1;    // Habilitar escritura a EEPROM (datos)
    
    INTCONbits.GIE = 0;     // Deshabilitar interrupciones
    
    EECON2 = 0x55;          // Secuencia obligatoria
    EECON2 = 0xAA;
    EECON1bits.WR = 1;      // Escribir
    
    while(PIR2bits.EEIF==0);
    PIR2bits.EEIF = 0;
    
    INTCONbits.GIE = 1;     // Habilitar interrupciones
    EECON1bits.WREN = 0;    // Deshabilitar escritura de EEPROM

}

unsigned char read(unsigned char   address){
    EEADR = address;        // direccion a leer
    EECON1bits.EEPGD = 0;   // memoria de datos
    EECON1bits.RD = 1;      // hace una lectura
    unsigned char  data = EEDAT;   // guardar el dato leído de EEPROM
    //__delay_ms(20);
    return data;
}

void enviocaracter(char a){
    while (TXSTAbits.TRMT == 0){
       
    }
    if (PIR1bits.TXIF){
            TXREG = a;
        }  
}

void enviocadena(char* cadena){
    while (*cadena != 0){
      enviocaracter(*cadena);
      cadena++;
    }
    if (PIR1bits.TXIF){
            TXREG = 13;
        } 
}

//**Function to get one byte of date from UART**//
char UART_get_char(){
    if(OERR) // check for Error 
    {
        CREN = 0; //If error -> Reset 
        CREN = 1; //If error -> Reset 
    }
    
    while(!RCIF);  // hold the program till RX buffer is free
    
    return RCREG; //receive the value and send it to main function
}
//_____________End of function________________//