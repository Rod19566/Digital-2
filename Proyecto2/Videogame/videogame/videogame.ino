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

//extern uint8_t menuBackground[];
extern uint8_t P1Guitar[];
extern uint8_t P2Guitar[];
extern uint8_t verticalArrow[];
//extern uint8_t fireBall[];
extern uint8_t gamingBackground[];

#define redLED RED_LED
#define blueLED BLUE_LED
#define greenLED GREEN_LED

// set pin numbers:
const int buttonPin = PUSH1;     // the number of the pushbutton pin
const int buttonPin1 = PUSH2;     // the number of the pushbutton pin

#define button1P1 PE_2     // Button for Player 1
#define button2P1 PE_3     // Button for Player 1
#define button3P1 PA_7     // Button for Player 1

#define button1P2 PD_6     // Button for Player 2
#define button2P2 PD_7     // Button for Player 2
#define button3P2 PF_4     // Button for Player 2

//define button variables
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin
 // the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonState1;             // the current reading from the input pin
int lastButtonState1 = HIGH;   // the previous reading from the input pin
long lastDebounceTime1 = 0;  // the last time the output pin was toggled

int buttonState1P1;             // the current reading from the input pin
int lastButtonState1P1 = HIGH;   // the previous reading from the input pin
long lastDebounceTime1P1 = 0;  // the last time the output pin was toggled

int buttonState2P1;             // the current reading from the input pin
int lastButtonState2P1 = HIGH;   // the previous reading from the input pin
long lastDebounceTime2P1 = 0;  // the last time the output pin was toggled

int buttonState3P1;             // the current reading from the input pin
int lastButtonState3P1 = HIGH;   // the previous reading from the input pin
long lastDebounceTime3P1 = 0;  // the last time the output pin was toggled

int onOff = 0;
int onOff1 = 0;
int onOff2 = 0;
int on2Off = 0;
int on2Off1 = 0;
int on2Off2 = 0;

int xStart = 0;
//define variables
int mode = 0; //0 = Menu; 1 = Players Select; 2 = Gaming; 3 = Ranks
int menuOptions = 0;
int menuMode = 0;
int playersMode = 0;
int gamingMode = 0;
int ranksMode = 0;
int timer = 0;
int totalTime = 60000 ; // (ms) --- delay(15) --- 15 ms 66.67 fps

//Prototypes
void menuScreen(void);
void checkButton1(String buttonMode);
void checkButton2(String buttonMode, int optionSelected);
void fireBallSelect(void);

struct Player{
  String name;
  int points;
};

struct Piece{
  int posX;
  int posY;
  int tick;       // 0 = fail; 1 = ok; 2 = perfect
  int pressed[2];    //determines if it has already been pressed by Player 1 [0] or Player 2 [1]
  char color[512];
};

Piece currentPiece;

Piece playingPieces[15];

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
  
  pinMode(button1P1, INPUT_PULLUP);  
  pinMode(button2P1, INPUT_PULLUP);  
  pinMode(button3P1, INPUT_PULLUP); 
  pinMode(button1P2, INPUT_PULLUP);  
  pinMode(button2P2, INPUT_PULLUP);  
  pinMode(button3P2, INPUT_PULLUP);  
  
  pinMode(redLED, OUTPUT);     
  pinMode(blueLED, OUTPUT);     
  pinMode(greenLED, OUTPUT);  

  //FillRect(0, 0, 320, 240, 0x000000);
  
//  LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);



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
          resetGame();
      }
      break;
        case 1:{ 
          //checkButton2("ranksmenu", 0);
          checkButton1P1();
          checkButton2P1();
          checkButton3P1();
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
          fireBallSelect(200);
      }
      break; 
      }
    }
    break; // end mode case:3 -- ranks
  } // end switch(mode)

} // loop

void resetGame(void){ //
  timer = 0;
  currentPiece.posX = 0;
  currentPiece.posY = 0;
  currentPiece.tick = 0;
  currentPiece.pressed[0] = 0;
  currentPiece.pressed[1] = 0;
  for (int num = 0; num < 512; num++){
    currentPiece.color[num] = redPiece[num];          
  }
  pieceRandomizer();
}

void menuScreen(void){
  //  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //  LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  //  LCD_Bitmap(0, 0, 320, 240, menuBackground);
    //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
  
  printGuitars(125, 0);
  
  LCD_Print("Guitar", 15, 30, 2, 0xffff, 0);
  LCD_Print("Game", 15, 50, 2, 0xffff, 0);
  
  LCD_Print("Play", 20, 90, 1, 0xffff, 0);
  LCD_Print("Scoreboard", 20, 115, 1, 0xffff, 0);
  
}

void ranksScreen(void){
  //  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //  LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  //  LCD_Bitmap(0, 0, 320, 240, menuBackground);

  printGuitars(125, 0);
  
  LCD_Print("Leader", 15, 30, 2, 0xffff, 0);
  LCD_Print("Board", 15, 50, 2, 0xffff, 0);
  
  LCD_Print("1. ", 15, 80, 1, 0xffff, 0);
  LCD_Print("2. ", 15, 100, 1, 0xffff, 0);
  LCD_Print("3. ", 15, 120, 1, 0xffff, 0);
  
  LCD_Print("Main Menu", 20, 200, 1, 0xffff, 0);
}

void fireBallSelect(int menuOption){
   for(int x = 0; x < 4; x++){
    delay(10);
    int anim2 = x%2;
    int anim3 = x%3;
    //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    if (menuOption == 0) {    //1st position  
      FillRect(5, 115, 10, 15, 0x0000);
      LCD_Sprite(5, 90, 10, 15, fireBall, 2, anim2, 0, 0);
    }
    else if (menuOption == 1) {     //2nd position       
      FillRect(5, 90, 10, 15, 0x0000);
      LCD_Sprite(5, 115, 10, 15, fireBall, 2, anim2, 0, 0);
    }
    else {     //3rd position    
      LCD_Sprite(5, menuOption, 10, 15, fireBall, 2, anim2, 0, 0);
    }
   } //for(int x = 0; x < 4; x++)
}


void playersScreen(void){
  //  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //  LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  //LCD_Bitmap(0, 0, 320, 240, menuBackground);
  FillRect(0, 0, 320, 240, 0x000000);
  
  printGuitars(150, 0);
  
  LCD_Print("Players", 15, 30, 2, 0xffff, 0);
  LCD_Print("Select", 15, 50, 2, 0xffff, 0);
  
  LCD_Print("1 Player", 20, 90, 1, 0xffff, 0);
  LCD_Print("2 Players", 20, 115, 1, 0xffff, 0);
  
}

void gamingScreen(void){
  //  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //  LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  //LCD_Bitmap(0, 0, 320, 240, gamingBackground);

  printGuitars(0,0);
  int fontSize = 2;
  if (menuOptions == 0){//1 Player
    xStart = 115;
    printFret(xStart);
    FillRect(0, 0, xStart, 150, 0x00000);
  }
  else if (menuOptions == 1){//2 Players
    fontSize = 1;
    xStart = 55;
    printFret(xStart);
    printFret(xStart + 130);
    FillRect(0, 0, xStart, 178, 0x00000);
    LCD_Print("P2", 5, 90, 2, 0xffff, 0);
    LCD_Print("Score", 5, 110, fontSize, 0xffff, 0);
  }  
  LCD_Print("P1", 5, 10, 2, 0xffff, 0);
  LCD_Print("Score", 5, 30, fontSize, 0xffff, 0);
} //gamingscreen

void printFret(int x){ //x being where it starts
  //V_line(unsigned int x, unsigned int y, unsigned int length, unsigned int color) 
  FillRect(x, 0, 120, 240, 0x8F4200);
  LCD_Sprite(x + 1, 220, 39, 19, redPiece, 1, 0, 0, 0);  // 39 x 38
  LCD_Sprite(x + 41, 220, 39, 19, yellowPiece, 1, 0, 0, 0);  // 39 x 38
  LCD_Sprite(x + 81, 220, 39, 19, greenPiece, 1, 0, 0, 0);  // 39 x 38
  V_line(x, 0, 240, 0xFCAB01);
  for (int i = 1; i < 3; i++){
    V_line(x+(i*40), 0, 240, 0x793800);
  }
  V_line(x + 120, 0, 240, 0xFCAB01);
}

void printGuitars(int xStart, int yStart){
  FillRect(0, 0, 320, 240, 0x000000);
  for(int x = xStart; x < 310; x++){
    for(int y = yStart + 5; y < 230; y++){
      LCD_Sprite(x, y, 14, 26, P1Guitar, 1, 1, 0, 0);
      LCD_Sprite(x, y + 28, 14, 28, P2Guitar, 1, 1, 0, 0);
      y += 57;
  }
      x += 15;
 }
}
void pieceRandomizer(void){
  /*
LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){ 
Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) 

struct Piece{
  int posY;
  int tick;   // 0 = fail; 1 = ok; 2 = perfect
  char color[];
};
   for(int x = 0; x <320-30; x++){
    delay(15);
    int anim2 = (x/11)%2;
    
    LCD_Sprite(x, 20, 30, 30, greenPiece, 5, anim2, 0, 0);  // 39 x 38
    V_line( x - 1, 20, 30, 0x00000);
  }*/
  
  for (int i = 0; i < 15; i++){
    playingPieces[i].posX = 0;
    playingPieces[i].posY = 0;
    playingPieces[i].tick = 0;   // 0 = fail; 1 = ok; 2 = perfect
    playingPieces[i].pressed[0] = 0;   //Player 1 0 = not pressed yet; 1 = already pressed
    playingPieces[i].pressed[1] = 0;   //Player 2 0 = not pressed yet; 1 = already pressed
    int randomNumber = random(3);
    switch(randomNumber){
      case 0: {
        for (int num = 0; num < 512; num++){
          playingPieces[i].color[num] = redPiece[num];
        } //for
      }
      break;
      case 1: {
        for (int num = 0; num < 512; num++){
          playingPieces[i].color[num] = yellowPiece[num];          
        } //for
      }
      break;
      case 2: {
        for (int num = 0; num < 512; num++){
          playingPieces[i].color[num] = greenPiece[num];
        } //for
      }
      break;
      default: {
        for (int num = 0; num < 512; num++){
          playingPieces[i].color[num] = redPiece[num];          
        } //for
      }
      break;
    }    
  }
}// void pieceRandomizer

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
      case 1:{ //scoreboard -- AKA go to main menu
        mode = 3;     
        ranksMode = 0;   
        menuOptions = 0;
      }
        break;
    }
  } else if (buttonMode == "playermenu"){ //
    mode = 2;
  }
  else if (buttonMode == "ranksmenu"){ //
    mode = 0;
    menuMode = 0;
  } 
  else if (buttonMode == "gaming"){
    mode = 0;
    menuMode = 0;
    menuOptions = 0;  
    gamingMode = 0;  
  }
    lastDebounceTime1 = millis();
  } 
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    buttonState1 = reading1;
  }
  lastButtonState1 = reading1;
}


void checkButton1P1(void){ //mode indicates response
  int reading = digitalRead(button1P1);
  if (reading == HIGH && lastButtonState1P1 == LOW) {
  //button action here
    if (!onOff){
      digitalWrite(redLED, HIGH);   // turn the LED on (HIGH is the voltage level)   
      onOff = 1;   
    } else{
      digitalWrite(redLED, LOW);   // turn the LED on (HIGH is the voltage level)   
      onOff = 0;      
    }
    LCD_Sprite(xStart + 1, 220, 39, 19, redPiece, 1, 19, 0, 0);  // 39 x 38
    delay(2*debounceDelay);
    lastDebounceTime1P1 = millis();
    LCD_Sprite(xStart + 1, 220, 39, 19, redPiece, 1, 0, 0, 0);  // 39 x 38
  } // if reading == HIGH
  if ((millis() - lastDebounceTime1P1) > debounceDelay) {
    buttonState1P1 = reading;
  }
  lastButtonState1P1 = reading;
}

void checkButton2P1(void){ //mode indicates response
  int reading = digitalRead(button2P1);
  if (reading == HIGH && lastButtonState2P1 == LOW) {
  //button action here
    if (!onOff1){
      digitalWrite(greenLED, HIGH);   // turn the LED on (HIGH is the voltage level)   
      onOff1 = 1;   
    } else{
      digitalWrite(greenLED, LOW);   // turn the LED on (HIGH is the voltage level)   
      onOff1 = 0;      
    }
    LCD_Sprite(xStart + 41, 220, 39, 19, yellowPiece, 1, 19, 0, 0);  // 39 x 38
    delay(2*debounceDelay);
    lastDebounceTime2P1 = millis();
    LCD_Sprite(xStart + 41, 220, 39, 19, yellowPiece, 1, 0, 0, 0);  // 39 x 38
  } // if reading == HIGH
  if ((millis() - lastDebounceTime2P1) > debounceDelay) {
    buttonState2P1 = reading;
  }
  lastButtonState2P1 = reading;
}

void checkButton3P1(void){ //mode indicates response
  int reading = digitalRead(button3P1);
  if (reading == HIGH && lastButtonState3P1 == LOW) {
  //button action here
    if (!onOff2){
      digitalWrite(blueLED, HIGH);   // turn the LED on (HIGH is the voltage level)   
      onOff2 = 1;   
    } else{
      digitalWrite(blueLED, LOW);   // turn the LED on (HIGH is the voltage level)   
      onOff2 = 0;      
    }
    LCD_Sprite(xStart + 81, 220, 39, 19, greenPiece, 1, 19, 0, 0);  // 39 x 38
    delay(2*debounceDelay);
    lastDebounceTime3P1 = millis();
    LCD_Sprite(xStart + 81, 220, 39, 19, greenPiece, 1, 0, 0, 0);  // 39 x 38
  } // if reading == HIGH
  if ((millis() - lastDebounceTime3P1) > debounceDelay) {
    buttonState3P1 = reading;
  }
  lastButtonState3P1 = reading;
}
