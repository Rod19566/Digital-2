//***************************************************************************************************************************************
/* Proyecto 2 VideoJuego
 * IE3027: Electrónica Digital 2 - 2023
 * Dina Rodríguez
 */
//***************************************************************************************************************************************


#include <stdint.h>
#include <SPI.h>
#include <SD.h>

File myFile;

#include "lcd_setup.h".
#include "sd_setup.h".

void setup() {
  // put your setup code here, to run once:
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  sd_Init();
  LCD_Init();
  LCD_Clear(0x00);
  
  FillRect(0, 0, 319, 206, 0x421b);
  String text1 = "Guitar Game";
  LCD_Print(text1, 75, 100, 2, 0xffff, 0x421b);
//  LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    
//  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //LCD_Bitmap(0, 0, 320, 240, fondo);
  
  for(int x = 0; x <319; x++){
    LCD_Bitmap(x, 52, 16, 16, tile2);
    LCD_Bitmap(x, 68, 16, 16, tile);
    
    LCD_Bitmap(x, 207, 16, 16, tile);
    LCD_Bitmap(x, 223, 16, 16, tile);
    x += 15;
 }

}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
