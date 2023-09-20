/*
  Blink
  The basic Energia example.
  Turns on an LED on for one second, then off for one second, repeatedly.
  Change the LED define to blink other LEDs.
  
  Hardware Required:
  * LaunchPad with an LED
  
  This example code is in the public domain.
*/
// set pin numbers:
const int buttonPin = PUSH1;     // the number of the pushbutton pin
const int buttonPin1 = PUSH2;     // the number of the pushbutton pin
#define redLED RED_LED
#define blueLED BLUE_LED
#define greenLED GREEN_LED

#define PD0 PD_0
#define PD1 PD_1
#define PD2 PD_2
#define PD3 PD_3
#define PE1 PE_1
#define PE2 PE_2
#define PE3 PE_3
#define PF1 PF_1

#define PF2 PF_2
#define PF3 PF_3
#define PB3 PB_3
#define PC4 PC_4
#define PC5 PC_5
#define PC6 PC_6
#define PC7 PC_7
#define PD6 PD_6


// Variables will change:
int inGame = 0;
int counter1 = 0;
int counter2 = 0;

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

 // the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonState1;             // the current reading from the input pin
int lastButtonState1 = LOW;   // the previous reading from the input pin

 // the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime1 = 0;  // the last time the output pin was toggled
long debounceDelay1 = 50;    // the debounce time; increase if the output flickers

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(PD0, OUTPUT);   // Set PF1 as an output
  pinMode(PD1, OUTPUT);   // Set PF1 as an output
  pinMode(PD2, OUTPUT);   // Set PF1 as an output
  pinMode(PD3, OUTPUT);   // Set PF1 as an output
  pinMode(PE1, OUTPUT);   // Set PF1 as an output
  pinMode(PE2, OUTPUT);   // Set PF1 as an output
  pinMode(PE3, OUTPUT);   // Set PF1 as an output
  
  pinMode(PF2, OUTPUT);   // Set PF1 as an output
  pinMode(PF3, OUTPUT);   // Set PF1 as an output
  pinMode(PB3, OUTPUT);   // Set PF1 as an output
  pinMode(PC4, OUTPUT);   // Set PF1 as an output
  pinMode(PC5, OUTPUT);   // Set PF1 as an output
  pinMode(PC6, OUTPUT);   // Set PF1 as an output
  pinMode(PC7, OUTPUT);   // Set PF1 as an output
  pinMode(PD6, OUTPUT);   // Set PF1 as an output

  allLedsOff();
  
  // initialize the digital pin as an output.  
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(buttonPin1, INPUT_PULLUP);  
  pinMode(redLED, OUTPUT);     
  pinMode(blueLED, OUTPUT);     
  pinMode(greenLED, OUTPUT);  
  newGame();
  delay(3000);               // wait for a second
}

// the loop routine runs over and over again forever:
void loop() {            // wait for a second
  checkButton1();
  checkButton2();
  compareWinner();
  updateLights();
}

void newGame(){
  whiteLIGHT();
  allLedsOff();
  inGame = 0;
  counter1 = 0;
  counter2 = 0;  
}

void countDown(){
  newGame();
  digitalWrite(redLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(greenLED, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(blueLED, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(greenLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                // wait for a second
  digitalWrite(redLED, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                // wait for a second
  inGame = 1;  
}

void checkButton1(){
  int reading = digitalRead(buttonPin);
  
  if (reading == HIGH && lastButtonState == LOW) {
  if (inGame == 0) {
    countDown();  
  } else{  
    counter1++;
  }
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonState = reading;
  }
  
  lastButtonState = reading;
}

void checkButton2(){
  int reading1 = digitalRead(buttonPin1);
  
  if (reading1 == HIGH && lastButtonState1 == LOW) {
  if (inGame == 0) {
    countDown();  
  } else{  
    counter2++;
  }
    lastDebounceTime1 = millis();
  } 
  
  if ((millis() - lastDebounceTime1) > debounceDelay1) {
    buttonState1 = reading1;
  }
  
  lastButtonState1 = reading1;
}

void compareWinner(){
  if (counter1 == 8){
    offLIGHT();
    digitalWrite(blueLED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(5000);               // wait for a second
    whiteLIGHT(); 
    newGame();
  }
  if (counter2 == 8){
    offLIGHT();
    inGame = 0; //variable that ends the round
    digitalWrite(greenLED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(5000);               // wait for a second
    whiteLIGHT();
    newGame();
  }
  
}

void whiteLIGHT(){
  digitalWrite(redLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(greenLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(blueLED, HIGH);   // turn the LED on (HIGH is the voltage level) 
}

void offLIGHT(){
  digitalWrite(redLED, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(greenLED, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(blueLED, LOW);   // turn the LED on (HIGH is the voltage level) 
}

void updateLights(){
  allLedsOff();
    switch(counter1){
      case 0: digitalWrite(PD0, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 1: digitalWrite(PD1, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 2: digitalWrite(PD2, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 3: digitalWrite(PD3, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 4: digitalWrite(PE1, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 5: digitalWrite(PE2, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 6: digitalWrite(PE3, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 7: digitalWrite(PF1, HIGH);  // Set PF1 HIGH (turn on)
      break; 
    }
    switch(counter2){
      case 0: digitalWrite(PF2, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 1: digitalWrite(PF3, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 2: digitalWrite(PB3, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 3: digitalWrite(PC4, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 4: digitalWrite(PC5, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 5: digitalWrite(PC6, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 6: digitalWrite(PC7, HIGH);  // Set PF1 HIGH (turn on)
      break;
      case 7: digitalWrite(PD6, HIGH);  // Set PF1 HIGH (turn on)
      break; 
    }
  }

void allLedsOff(){
  digitalWrite(PD0, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PD1, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PD2, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PD3, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PE1, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PE2, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PE3, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PF1, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PF2, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PF3, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PB3, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PC4, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PC5, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PC6, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PC7, LOW);  // Set PF1 HIGH (turn on)
  digitalWrite(PD6, LOW);  // Set PF1 HIGH (turn on)
  
}
