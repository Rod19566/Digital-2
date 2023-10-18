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
#include "songs.h"

extern uint8_t P1Guitar[];
extern uint8_t P2Guitar[];
extern uint8_t verticalArrow[];
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

#define BUZZER_PIN PF_2

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
int toc = 0;
int finalP1 = 0;
int finalP2 = 0;
const int amountPieces = 20;
const int spacePerPiece = 30;
const int phase = 10;
const int totalTime = 1000 ; // (ms) --- delay(15) --- 15 ms 66.67 fps

//Prototypes
void menuScreen(void);
void checkButton1(String buttonMode);
void checkButton2(String buttonMode, int optionSelected);
void checkButton1P1(void);
void checkButton2P1(void);
void checkButton3P1(void);
void fireBallSelect(void);
//void piecePrinter(Piece p2Print, int timerPrint);


struct Piece{
  int posX;
  int posY;
  int posYStart;
  int id;
  int tick;       // 0 = fail; 1 = ok; 2 = perfect
  int pressed[2];    //determines if it has already been pressed by Player 1 [0] or Player 2 [1]
  String colorName;
};
  Piece currentPiece;
  Piece playingPieces[amountPieces];
  Player P1;
  Player P2;
  Player recordPlayers[3];
  String content;
  
  ///////////////////////////////SETUP//////////////////////
void setup() {
  // put your setup code here, to run once:
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SPI.setModule(0);
  pinMode(12, OUTPUT);
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
  
  pinMode(BUZZER_PIN, OUTPUT); 

}//setup//////////////////////////////////////////////////

///////////////////MAIN LOOP/////////////////////////////
void loop() {
  // put your main code here, to run repeatedly: 
  switch(mode){//0 = Menu; 1 = Players Select; 2 = Gaming; 3 = Ranks
    case 0:{ //menu
      switch (menuMode){//setup menu
        case 0:{
          menuScreen();
          menuOptions = 0;
          menuMode = 1;
          toc = 0;
      }
      break;
        case 1:{ //menu select
          fireBallSelect(menuOptions);
          checkButton1("mainmenu");
          checkButton2("mainmenu", menuOptions);
          playMusic(toc++);
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
          pieceRandomizer();
      }
      break;
        case 1:{ //in game
          while (timer <= totalTime && gamingMode == 1){
            checkButton2("gotoMainMenu", 0);
            //if (timer % 50 == 0) playMusic(timer/100);
            checkButton1P1();
            checkButton2P1();
            checkButton3P1();
            piecePrinter(playingPieces[0], timer);
            piecePrinter(playingPieces[1], timer);
            piecePrinter(playingPieces[2], timer);
            if (timer % 30 == 0){
              playMusic1(timer/100);
              for (int i = 0; i < amountPieces; i++){ //prints pieces
                if (playingPieces[i].posY >= 0 && playingPieces[i].posY < 240 && playingPieces[i].pressed[0] == 0) {
                  piecePrinter(playingPieces[i], timer);
                }
                playingPieces[i].posY = playingPieces[i].posY + spacePerPiece;
              if (playingPieces[i].posY >= 230) {
                currentPiece = playingPieces[i];
                playingPieces[i].posY = playingPieces[i].posYStart;
                playingPieces[i].pressed[0] = 0;
              }
              } //for       
            }
            printScores();
            //int wait = 10;
            //delay(wait);
            timer++;            
          } 
          finalP1 = P1.points;
          finalP2 = P2.points;
          resultsScreen(String(finalP1), String(finalP2));
          gamingMode = 2;
      }
      break; 
        case 2:{ //winners screen
            int wait = 5000;
            checkButton2("gotoMainMenu", 0); 
      }
      break;
      }
    }
    break; // end mode case:2 -- Gaming
    
    case 3:{//ranks
      switch (ranksMode){//setup menu
        case 0:{
          //readFile("scoreb~1.txt", content);
          int numPlayers = 0;          
          readFile("scoreb~1.txt", recordPlayers, numPlayers);

          ranksScreen();
          ranksMode = 1;
      }
      break;
        case 1:{ 
          checkButton2("gotoMainMenu", 0);
          fireBallSelect(200);
      }
      break; 
      }
    }
    break; // end mode case:3 -- ranks
  } // end switch(mode)

} // loop////////////////////////////////////////

////////////////////FUNCTIONS//////////////////
void resetGame(void){ //
  timer = 0;
  P1.name = chooseRandomName();
  P1.identifier = "P1: " + P1.name;
  P1.points = 0;
  P2.name = chooseRandomName();
  P2.identifier = "P2: " + P2.name;
  P2.points = 0;
}

void menuScreen(void){
  printGuitars(125, 0);
  
  LCD_Print("Guitar", 15, 30, 2, 0xffff, 0);
  LCD_Print("Game", 15, 50, 2, 0xffff, 0);
  
  LCD_Print("Play", 20, 90, 1, 0xffff, 0);
  LCD_Print("Scoreboard", 20, 115, 1, 0xffff, 0);
  
}

String chooseRandomName(void){
  int randomNumber = random(10);
  randomNumber = random(10);
  randomNumber = random(10);
  switch(randomNumber){
    case 0: return "Sam";
      break;
    case 1: return "Alex";
      break;
    case 2: return "Charlie";
      break;
    case 3: return "Riley";
      break;
    case 4: return "Taylor";
      break;
    case 5: return "Peyton";
      break;
    case 6: return "Frankie";
      break;
    case 7: return "Robin";
      break;
    case 8: return "Jamie";
      break;
    case 9: return "Morgan";
      break;
  }
}

void ranksScreen(void){
  //  LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //  LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  //  LCD_Bitmap(0, 0, 320, 240, menuBackground);

  printGuitars(125, 0);
  
  LCD_Print("Leader", 15, 30, 2, 0xffff, 0);
  LCD_Print("Board", 15, 50, 2, 0xffff, 0);
  
  LCD_Print("1. ", 15, 80, 1, 0xffff, 0);
  LCD_Print(recordPlayers[0].identifier, 25, 80, 1, 0xffff, 0);
  LCD_Print(String(recordPlayers[0].points), 25, 95, 1, 0xffff, 0);
  LCD_Print("2. ", 15, 120, 1, 0xffff, 0);
  LCD_Print(recordPlayers[1].identifier, 25, 120, 1, 0xffff, 0);
  LCD_Print(String(recordPlayers[1].points), 25, 135, 1, 0xffff, 0);
  LCD_Print("3. ", 15, 150, 1, 0xffff, 0);
  LCD_Print(recordPlayers[2].identifier , 25, 150, 1, 0xffff, 0);
  LCD_Print(String(recordPlayers[2].points) , 25, 165, 1, 0xffff, 0);
  
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

void resultsScreen(String pointsP1, String pointsP2){
  int posYP1 = 45;
  printGuitars(0,0);
  FillRect(25, 25, 270, 170, 0x00000);
  if (menuOptions == 0){//1 Player
    posYP1 = 90;
  }
  else if (menuOptions == 1){//2 Players
    LCD_Sprite(50, 100, 14, 28, P2Guitar, 1, 1, 0, 0);
    LCD_Print(P2.identifier, 70, 105, 2, 0xffff, 0);
    LCD_Print("Score", 75, 125, 2, 0xffff, 0);
    LCD_Print(pointsP2, 175, 125, 2, 0xffff, 0);
  }  
    LCD_Sprite(50, posYP1, 14, 28, P1Guitar, 1, 1, 0, 0);
    LCD_Print(P1.identifier, 70, posYP1 + 5, 2, 0xffff, 0);
    LCD_Print("Score", 75, posYP1 + 30, 2, 0xffff, 0);
    LCD_Print(pointsP1, 175, posYP1 + 30, 2, 0xffff, 0);

    //myFile = SD.open("/");
    //printDirectory(myFile, 0);
    writeFile(myFile, P1.identifier + "=" + pointsP1);  
    writeFile(myFile, P2.identifier + "=" + pointsP2);  
}

void gamingScreen(void){
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
    LCD_Sprite(38, 85, 14, 28, P2Guitar, 1, 1, 0, 0);
    LCD_Print("Score", 5, 110, fontSize, 0xffff, 0);
  }  
  LCD_Print("P1", 5, 10, 2, 0xffff, 0);
  LCD_Sprite(38, 5, 14, 26, P1Guitar, 1, 1, 0, 0);
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
  for (int i = 0; i < amountPieces; i++){
    playingPieces[i].id = i;  
    playingPieces[i].posY = -(spacePerPiece * playingPieces[i].id) + phase;
    playingPieces[i].posYStart = playingPieces[i].posY;  
    playingPieces[i].tick = 0;   // 0 = fail; 1 = ok; 2 = perfect
    playingPieces[i].pressed[0] = 0;   //Player 1 0 = not pressed yet; 1 = already pressed
    playingPieces[i].pressed[1] = 0;   //Player 2 0 = not pressed yet; 1 = already pressed
    int randomNumber = random(3);
    switch(randomNumber){
      case 0: {
        playingPieces[i].colorName = "red";
          if (menuOptions == 0){//1 Player
            playingPieces[i].posX = xStart + 1;
          }
          else if (menuOptions == 1){//2 Players
          }
      }
      break;
      case 1: {
        playingPieces[i].colorName = "yellow";
          if (menuOptions == 0){//1 Player
            playingPieces[i].posX = xStart + 41;
          }
          else if (menuOptions == 1){//2 Players
          }
      }
      break;
      case 2: {
        playingPieces[i].colorName = "green";
          if (menuOptions == 0){//1 Player
            playingPieces[i].posX = xStart + 81;
          }
          else if (menuOptions == 1){//2 Players
          }
      }
      break;
    }    
  }
}// void pieceRandomizer


void piecePrinter(Piece p2Print, int timerPrint){
  //  LCD_Sprite(xStart + 1, 220, 39, 19, redPiece, 1, 0, 0, 0);  // 39 x 38
   // LCD_Sprite(xStart + 41, 220, 39, 19, yellowPiece, 1, 0, 0, 0);  // 39 x 38
   // LCD_Sprite(xStart + 81, 220, 39, 19, greenPiece, 1, 0, 0, 0);  // 39 x 38
   //printFret(xStart);
  if (p2Print.pressed[0] == 0){ //if not pressed keep moving
    if (p2Print.colorName == "red"){
      FillRect(xStart + 1, p2Print.posY - spacePerPiece, 39, 19, 0x8F4200);
      LCD_Sprite(xStart + 1, p2Print.posY, 39, 19, redPiece, 1, 0, 0, 0);  // 39 x 38
    }
    if (p2Print.colorName == "yellow"){
      FillRect(xStart + 41, p2Print.posY - spacePerPiece, 39, 19, 0x8F4200);
      LCD_Sprite(xStart + 41, p2Print.posY, 39, 19, yellowPiece, 1, 0, 0, 0);  // 39 x 38
    }
    if (p2Print.colorName == "green"){
      FillRect(xStart + 81, p2Print.posY - spacePerPiece, 39, 19, 0x8F4200);
      LCD_Sprite(xStart + 81, p2Print.posY, 39, 19, greenPiece, 1, 0, 0, 0);  // 39 x 38
    }   
  }
  
} //void piece printer 

void printScores(void){
  if (menuOptions == 0){
    LCD_Print(P1.name, 5, 50, 1, 0xffff, 0);
    LCD_Print(String(P1.points), 20, 75, 2, 0xffff, 0);
    
    LCD_Print("Time", 40, 135, 1, 0xffff, 0);
    LCD_Print(String(timer), 75, 135, 1, 0xffff, 0);
  }
  if (menuOptions == 1){
    LCD_Print(P1.name, 0, 45, 1, 0xffff, 0);
    LCD_Print(String(P1.points), 5, 60, 2, 0xffff, 0);
    LCD_Print(P2.name, 0, 120, 1, 0xffff, 0);
    LCD_Print(String(P2.points), 5, 135, 2, 0xffff, 0);
    
    LCD_Print("Time", 5, 160, 1, 0xffff, 0);
    LCD_Print(String(timer), 5, 170, 1, 0xffff, 0);
  }
}

///////////////////BUTTONS//////////////////////////////////////
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
        menuMode = 0;
        ranksMode = 0;   
        menuOptions = 0;
      }
        break;
    }
  } else if (buttonMode == "playermenu"){ //
    mode = 2;
    menuMode = 0;
  }
  else if (buttonMode == "gotoMainMenu"){ //
    mode = 0;
    menuMode = 0;
    menuOptions = 0;
    gamingMode = 0; 
    playersMode = 0;
    //return;
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
    if (currentPiece.colorName == "red" && currentPiece.posY >= 210 && currentPiece.posY <=250) {
      P1.points++;
      currentPiece.pressed[0] = 1;
    }
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
    if (currentPiece.colorName == "yellow" && currentPiece.posY >= 210 && currentPiece.posY <=250) {
      P1.points++;
      currentPiece.pressed[0] = 1;
    }
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
     // digitalWrite(blueLED, HIGH);   // turn the LED on (HIGH is the voltage level)   
      onOff2 = 1;   
    } else{
      digitalWrite(blueLED, LOW);   // turn the LED on (HIGH is the voltage level)   
      onOff2 = 0;      
    }
    LCD_Sprite(xStart + 81, 220, 39, 19, greenPiece, 1, 19, 0, 0);  // 39 x 38
    delay(2*debounceDelay);
    lastDebounceTime3P1 = millis();
    LCD_Sprite(xStart + 81, 220, 39, 19, greenPiece, 1, 0, 0, 0);  // 39 x 38
    if (currentPiece.colorName == "green" && currentPiece.posY >= 210 && currentPiece.posY <=250 && currentPiece.pressed[0] == 0) {
      P1.points++;
      currentPiece.pressed[0] = 1;
    }
    
  } // if reading == HIGH
  if ((millis() - lastDebounceTime3P1) > debounceDelay) {
    buttonState3P1 = reading;
  }
  lastButtonState3P1 = reading;
}
