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
//extern uint8_t fireBall[];
extern uint8_t gamingBackground[];

// set pin numbers:
const int buttonPin = PUSH1;     // the number of the pushbutton pin
const int buttonPin1 = PUSH2;     // the number of the pushbutton pin

//define button variables
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin
 // the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonState1;             // the current reading from the input pin
int lastButtonState1 = HIGH;   // the previous reading from the input pin
 // the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime1 = 0;  // the last time the output pin was toggled
long debounceDelay1 = 50;    // the debounce time; increase if the output flickers


//define variables
int mode = 0; //0 = Menu; 1 = Players Select; 2 = Gaming; 3 = Ranks
int menuOptions = 0;
int menuMode = 0;
int playersMode = 0;
int gamingMode = 0;
int ranksMode = 0;

//Prototypes
void menuScreen(void);
void checkButton1(String buttonMode);
void checkButton2(String buttonMode, int optionSelected);
void fireBallSelect(void);

void setup() {
  // put your setup code here, to run once:
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SD_Init();
  LCD_Init();
  LCD_Clear(0x00);
  // initialize the digital pin as an output.  
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(buttonPin1, INPUT_PULLUP);  

  FillRect(0, 0, 320, 240, 0x000000);
  
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
  switch(mode){//0 = Menu; 1 = Players Select; 2 = Gaming; 3 = Ranks
    case 0:{ //menu
      switch (menuMode){//setup menu
        case 0:{
          menuScreen();
          menuOptions = 0;
          menuMode = 1;
      }
      break;
        case 1:{ //menu select
          fireBallSelect(menuOptions);
          checkButton1("mainmenu");
          checkButton2("mainmenu", menuOptions);
      }
      break; 
      }
      
    }
    break; // end mode case:0 -- MENU
    
    case 1:{//players select
      switch (playersMode){//setup menu
        case 0:{
          playersScreen();
          menuOptions = 0;
          playersMode = 1;
      }
      break;
        case 1:{ //number of players menu select
          fireBallSelect(menuOptions);
          checkButton1("playermenu");
          checkButton2("playermenu", menuOptions);
      }
      break; 
      }
    }
    break; // end mode case:1 -- Players Select
    
    case 2:{//gaming
      switch (gamingMode){//setup menu
        case 0:{
          gamingScreen();
          gamingMode = 1;
      }
      break;
        case 1:{ 

      }
      break; 
      }
    }
    break; // end mode case:2 -- Gaming
    
    case 3:{//ranks
      switch (ranksMode){//setup menu
        case 0:{
          ranksScreen();
          ranksMode = 1;
      }
      break;
        case 1:{ 
          checkButton2("ranksmenu", 0);
          fireBallSelect(2);
      }
      break; 
      }
    }
    break; // end mode case:3 -- ranks
  } // end switch(mode)

}

void menuScreen(void){
  //  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //  LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  LCD_Bitmap(0, 0, 320, 240, menuBackground);
  
  LCD_Print("Guitar", 15, 30, 2, 0xffff, 0);
  LCD_Print("Game", 15, 50, 2, 0xffff, 0);
  
  LCD_Print("Play", 20, 90, 1, 0xffff, 0);
  LCD_Print("Scoreboard", 20, 115, 1, 0xffff, 0);
  
}

void ranksScreen(void){
  //  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //  LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  LCD_Bitmap(0, 0, 320, 240, menuBackground);
  
  LCD_Print("Leader", 15, 30, 2, 0xffff, 0);
  LCD_Print("Board", 15, 50, 2, 0xffff, 0);
  
  LCD_Print("Main Menu", 20, 220, 1, 0xffff, 0);
  
}

void fireBallSelect(int menuOption){
   for(int x = 0; x < 4; x++){
    delay(10);
    int anim2 = x%2;
    //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    if (menuOption == 0) {    //1st position  
      FillRect(5, 115, 10, 15, 0x0000);
      LCD_Sprite(5, 90, 10, 15, fireBall, 2, anim2, 0, 0);
    }
    if (menuOption == 1) {     //2nd position       
      FillRect(5, 90, 10, 15, 0x0000);
      LCD_Sprite(5, 115, 10, 15, fireBall, 2, anim2, 0, 0);
    }
    if (menuOption == 2) {     //3rd position    
      LCD_Sprite(5, 220, 10, 15, fireBall, 2, anim2, 0, 0);
    }
   }
}


void playersScreen(void){
  //  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //  LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  LCD_Bitmap(0, 0, 320, 240, menuBackground);
  
  LCD_Print("Players", 15, 30, 2, 0xffff, 0);
  LCD_Print("Select", 15, 50, 2, 0xffff, 0);
  
  LCD_Print("1 Player", 20, 90, 1, 0xffff, 0);
  LCD_Print("2 Players", 20, 115, 1, 0xffff, 0);
  
}

void gamingScreen(void){
  //  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //  LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  //LCD_Bitmap(0, 0, 320, 240, gamingBackground);
  
      FillRect(0, 0, 320, 240, 0x0000);
  if (menuOptions == 0){//1 Player
  //V_line(unsigned int x, unsigned int y, unsigned int length, unsigned int color) 
    V_line(110, 0, 240, 0xffff);
    V_line(210, 0, 240, 0xffff);
  }
  else if (menuOptions == 1){//2 Players
  //V_line(unsigned int x, unsigned int y, unsigned int length, unsigned int color) 
    V_line(50, 0, 240, 0xffff);
    V_line(150, 0, 240, 0xffff);
    V_line(170, 0, 240, 0xffff);
    V_line(270, 0, 240, 0xffff);
  }
  
  
}


void movingArrows(void){
   for(int x = 0; x <320-30; x++){
    delay(15);
    int anim5 = (x/11)%5;
    
//LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){ 
//Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) 
    LCD_Sprite(x, 20, 30, 30, verticalArrow, 5, anim5, 0, 0);
    V_line( x - 1, 20, 30, 0x00000);
  }
  
  for(int x = 320-30; x >0; x--){
    delay(5);
    int anim5 = (x/11)%5;
    
    LCD_Sprite(x, 20, 30, 30, verticalArrow, 5, anim5, 1, 0);
    V_line( x + 30, 20, 30, 0x00000);
  } 
}


void checkButton1(String buttonMode){ //mode indicates response
  int reading = digitalRead(buttonPin);
  
  if (reading == HIGH && lastButtonState == LOW) {
  //button action here
  //************************************//
  if (buttonMode == "mainmenu" || buttonMode == "playermenu"){
      menuOptions++;
      if (menuOptions >= 2) menuOptions = 0;
  } else if (buttonMode == ""){
    
  }
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonState = reading;
  }
  
  lastButtonState = reading;
}


void checkButton2(String buttonMode, int optionSelected){
  int reading1 = digitalRead(buttonPin1);
  
  if (reading1 == HIGH && lastButtonState1 == LOW) {
  //button action here
  //************************************//
   //0 = Menu; 1 = Players Select; 2 = Gaming; 3 = Ranks
  if (buttonMode == "mainmenu"){ //
    switch(optionSelected){
      case 0:{ // play
        mode = 1;
        menuOptions = 0;
        menuMode = 0;
      }
        break;
      case 1:{ //scoreboard
        mode = 3;     
        ranksMode = 0;   
      }
        break;
      default:
        break;
    }
      
  } else if (buttonMode == "playermenu"){ //
    mode = 2;
  }
  else if (buttonMode == "ranksmenu"){ //
    mode = 0;
    menuMode = 0;
  }
    lastDebounceTime1 = millis();
  } 
  
  if ((millis() - lastDebounceTime1) > debounceDelay1) {
    buttonState1 = reading1;
  }
  
  lastButtonState1 = reading1;
}
