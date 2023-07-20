#define _XTAL_FREQ 8000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#include <xc.h>
#include "lcd.h"
//
//int main()
//{
//    unsigned int a;
//    TRISD = 0x00;
//    Lcd_Init();
//    while(1)
//    {
//        Lcd_Clear();
//        Lcd_Set_Cursor(1,1);
//        Lcd_Write_String("LCD Library for");
//        Lcd_Set_Cursor(2,1);
//        Lcd_Write_String("MPLAB XC8");
//        __delay_ms(2000);
//        Lcd_Clear();
//        Lcd_Set_Cursor(1,1);
//        Lcd_Write_String("Developed By");
//        Lcd_Set_Cursor(2,1);
//        Lcd_Write_String("electroSome");
//        __delay_ms(2000);
//        Lcd_Clear();
//        Lcd_Set_Cursor(1,1);
//        Lcd_Write_String("www.electroSome.com");
//
//        for(a=0;a<15;a++)
//        {
//            __delay_ms(300);
//            Lcd_Shift_Left();
//        }
//
//        for(a=0;a<15;a++)
//        {
//            __delay_ms(300);
//            Lcd_Shift_Right();
//        }
//
//        Lcd_Clear();
//        Lcd_Set_Cursor(2,1);
//        Lcd_Write_Char('e');
//        Lcd_Write_Char('S');
//        __delay_ms(2000);
//    }
//    return 0;
//}