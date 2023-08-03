  
#ifndef XC_LCD_H
#define	XC_LCD_H


#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h> 
#include <stdio.h>

#ifndef RS
#define RS RC0
#endif

#ifndef EN
#define EN RC1
#endif

#ifndef D0
#define D0 RD0
#endif

#ifndef D1
#define D1 RD1
#endif

#ifndef D2
#define D2 RD2
#endif

#ifndef D3
#define D3 RD3
#endif

#ifndef D4
#define D4 RD4
#endif

#ifndef D5
#define D5 RD5
#endif

#ifndef D6
#define D6 RD6
#endif

#ifndef D7
#define D7 RD7
#endif

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

