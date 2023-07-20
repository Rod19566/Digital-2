  
#ifndef XC_LCD_H
#define	XC_LCD_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>

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
void LCD_Test(void);

//LCD Functions Developed by electroSome

#endif	/* XC_LCD_H */

