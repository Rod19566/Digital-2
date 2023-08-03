/* 
 * File: LCD.h  
 * Se utilizó y se adaptaron las librerías de Ligo George 
 * de la página www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD8bits_H
#define	LCD8bits_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

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

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
//LCD Functions Developed by electroSome

unsigned char a = 0;
void Lcd_Port(unsigned char a);

void Lcd_Cmd(unsigned char a);

void Lcd_Clear(void);

void Lcd_Set_Cursor(unsigned char a,unsigned char b);

void Lcd_Init(void);

void Lcd_Write_Char(char a);

void Lcd_Write_String(char *a);

void Lcd_Shift_Right(void);

void Lcd_Shift_Left(void);

#endif	/* LCD_H */
