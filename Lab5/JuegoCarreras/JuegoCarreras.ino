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
#define redLED RED_LED
#define blueLED BLUE_LED
#define greenLED GREEN_LED


// Variables will change:
int startGO = 0;
int inGame = 0;
int counter1 = 0;
int counter2 = 0;

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

 // the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(redLED, OUTPUT);     
  pinMode(blueLED, OUTPUT);     
  pinMode(greenLED, OUTPUT);  
  pinMode(buttonPin, INPUT_PULLUP);  
  newGame();
  whiteLIGHT();
  delay(1000);               // wait for a second
}

// the loop routine runs over and over again forever:
void loop() {            // wait for a second
  checkButton1();
  if(inGame) {
    checkGame();    
  }
}

void newGame(){
  startGO = 0;
  inGame = 0;
  counter1 = 0;
  counter2 = 0;
  
}

void countDown(){
  newGame();
  digitalWrite(redLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(greenLED, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(blueLED, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(2000);               // wait for a second
  digitalWrite(greenLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                // wait for a second
  digitalWrite(redLED, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                // wait for a second
  whiteLIGHT();
  startGO = 0;
}

void checkButton1(){
  int reading = digitalRead(buttonPin);
  
  if (reading == HIGH && lastButtonState == LOW) {
    startGO = 1;
    inGame = 1;
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonState = reading;
  }
  
  lastButtonState = reading;
}



void whiteLIGHT(){
  
  digitalWrite(redLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(greenLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(blueLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  
}

void checkGame(){
  if (startGO) countDown();
  else{     //after start
    

    inGame = 0; //variable that ends the round
  }
  
}
