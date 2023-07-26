  
#ifndef XC_LCD_H
#define	XC_LCD_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h> 
#include <stdio.h>

/*
 Prototipos
 */
void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Clear();
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Lcd_Init(void);
void example(void);
void intToString(int num, char *buffer);
void Lcd_Write_Integer(int value);

//LCD Functions Developed by electroSome

#endif	/* XC_LCD_H */

