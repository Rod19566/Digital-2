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

#include "lcd_setup.h"
#include "sd_setup.h"

extern uint8_t menuBackground[];
extern uint8_t verticalArrow[];

void setup() {
  // put your setup code here, to run once:
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SD_Init();
  LCD_Init();
  LCD_Clear(0x00);
     
//  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  LCD_Bitmap(0, 0, 320, 240, menuBackground);
  
  //FillRect(0, 0, 319, 206, 0x00033d);
  String text1 = "Guitar Game";
  LCD_Print(text1, 75, 100, 2, 0xffff, 0);
//  LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
 /*
  for(int x = 0; x <319; x++){
    LCD_Bitmap(x, 52, 16, 16, tile2);
    LCD_Bitmap(x, 68, 16, 16, tile);
    
    LCD_Bitmap(x, 207, 16, 16, tile);
    LCD_Bitmap(x, 223, 16, 16, tile);
    x += 15;
 }
 */

}//setup

void loop() {
  
  // put your main code here, to run repeatedly: 
 movingArrows();

}

void movingArrows(void){
   for(int x = 0; x <320-30; x++){
    delay(15);
    int anim5 = (x/11)%5;
    
//void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){ 
//Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) 
    LCD_Sprite(x, 20, 30, 30, verticalArrow, 5, anim5, 4, 0);
    V_line( x - 1, 20, 30, 0x00000);
  }
  
  for(int x = 320-30; x >0; x--){
    delay(5);
    int anim5 = (x/11)%5;
    
    LCD_Sprite(x, 20, 30, 30, verticalArrow, 5, anim5, 1, 0);
    V_line( x + 30, 20, 30, 0x00000);
  } 
}
