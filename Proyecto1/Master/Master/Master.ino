/************************** Configuration ***********************************/

#include "config.h" 
#include <ESP32Servo.h>
#include <HardwareSerial.h>


#define SPTR_SIZE   2
char   *sPtr [SPTR_SIZE];

Servo myservo;  // create servo object to control a servo

// These constants won't change. They're used to give names to the pins used:
const int analogInPin0 = 25;  // Analog input pin that the potentiometer is attached to
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
const int servoPin = 26;
const int potPin = 34;
const int ledPin = 2;

HardwareSerial SerialPort(2);  //if using UART2

int adcValue = 0;
boolean ledState = false;
byte receivedValueSlave1 = 0;
String uartValue = "";
int sensorValue0 = 0;  // value read from the pot
int outputValue0 = 0;  // value output to the PWM (analog out)
int OnOff = 0;
int fanOnOff = 0;
int weight = 0; 
int ultrasonic = 0;

int packetsReceived = 0;


// Set up the 'Example1' feed
AdafruitIO_Feed *ultrasonicFeed = io.feed("example1");
AdafruitIO_Feed *ledFeed = io.feed("led");
AdafruitIO_Feed *trashWeightFeed = io.feed("slaveadc1");
AdafruitIO_Feed *powerFeed = io.feed("power");

void setup() {
  Serial.begin(115200);
  SerialPort.begin(9600, SERIAL_8N1, 16, 17); 
  
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 26 to the servo object
	// using default min/max of 1000us and 2000us
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Wait for serial monitor to open
  while (!Serial);

  Serial.print("Connecting to Adafruit IO");

  // Connect to io.adafruit.com
  io.connect();

  // Set up a message handler for the led feed
  ledFeed->onMessage(handleMessage);

  // Wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // We are connected
  Serial.println();
  Serial.println(io.statusText());
  
 // Save slave pot value to the 'slaveValue' feed on Adafruit IO
  Serial.print("Robot is ON/Off -> ");
  Serial.println("Powered but Off");
  powerFeed->save(1);
  delay(2000);

}

void loop() {
  int adcRawValue = analogRead(potPin);
  adcValue = map(adcRawValue, 0, 4095, 0, 255);

  digitalWrite(ledPin, ledState ? HIGH : LOW);
  setBasket(ledState);

    // Read UART data here
    readUART();
    delay(300);
    Serial.print("UART: ");
    Serial.println(uartValue);
    processReceivedData(uartValue);

  //setBasket(OnOff, fanOnOff);

  // Process Adafruit IO events
  io.run();
  sendADA();
  readADA();
  
}

// This function is called whenever a 'led' message is received from Adafruit IO
void handleMessage(AdafruitIO_Data *data) {
  char receivedAdafruitData = *data->value();

  Serial.print("Received <- ");
  Serial.println(receivedAdafruitData);

  ledState = (receivedAdafruitData == '1');
}


void sendADA(void){  
  // Save slave pot value to the 'slaveValue' feed on Adafruit IO
  Serial.print("Sending Trash Value value -> ");
  Serial.println(weight);
  trashWeightFeed->save(weight);
  delay(2000);
}

void readADA(void){
  // Adafruit IO is rate limited for publishing, so add a delay between feed->save events
  delay(2000);
  // Save ADC pot value to the 'adcValue' feed on Adafruit IO
  Serial.print("Sending Distance value -> ");
  Serial.println(ultrasonic);
  ultrasonicFeed->save(ultrasonic);
  delay(2000);
  
  // Save slave pot value to the 'slaveValue' feed on Adafruit IO
  Serial.print("Robot is ON/Off -> ");
  Serial.println(OnOff);
  powerFeed->save(OnOff);
  delay(2000);

}

void readUART(void){
  if (SerialPort.available())
  {
    String received = SerialPort.readStringUntil('\n');
    //String received = SerialPort.readString();
    uartValue = received;
    delay(300);
  }
}

void setBasket(int on){
  if (on == 1) myservo.write(0);    // tell servo to go to position in variable 'pos'
  else myservo.write(60);
  delay(100);  
}


//separates string into array dividing by spaces
int separate (String str, char   **p, int    size ){
    int  n;
    char s [100];

    strcpy (s, str.c_str ());

    *p++ = strtok (s, " ");
    for (n = 1; NULL != (*p++ = strtok (NULL, " ")); n++)
        if (size == n)
            break;

    return n;
}


//
//int OnOff = 0;
//int fanOnOff = 0;
//int weight = 0; 
//int ultrasonic = 0; 
void processReceivedData(String data) {
    // Split the received data into parts (command and value)
    int delimiterIndex = data.indexOf(' ');
    if (delimiterIndex != -1) {
        String command = data.substring(0, delimiterIndex);
        String valueStr = data.substring(delimiterIndex + 1);

        // Convert the value string to an integer
        int value = valueStr.toInt();

        // Perform actions based on the received command and value
        if (command.equals("Power")) {
          OnOff = value;
        } else if (command.equals("Fan")) {
          fanOnOff = value;
        } else if (command.equals("FSR")) {
          weight = value;
        } else if (command.equals("Ultrasonic")) {
          ultrasonic = value;
        }
    }
    
}

