/* 
 * File: usart.h  
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USART_H
#define	USART_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
//usart
void write(unsigned char data, unsigned char address);
unsigned char read(unsigned char address);
void enviocaracter(char a);
void enviocadena(char* cadena);
char UART_get_char();

#endif	/* USART_H */