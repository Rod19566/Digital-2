/************************** Configuration ***********************************/

#include "config.h"

const int potPin = 34;
const int ledPin = 2;
const int slave1Address = 0x50;

int adcValue = 0;
boolean ledState = false;
byte receivedValueSlave1 = 0;

// Set up the 'Example1' feed
AdafruitIO_Feed *adcMasterFeed = io.feed("example1");
AdafruitIO_Feed *ledFeed = io.feed("led");
AdafruitIO_Feed *adcSlave1Feed = io.feed("slaveadc1");

void setup() {
  Wire.begin(21,22);
  Serial.begin(115200);
  
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
}

void loop() {
  int adcRawValue = analogRead(potPin);
  adcValue = map(adcRawValue, 0, 4095, 0, 255);

  digitalWrite(ledPin, ledState ? HIGH : LOW);

  Wire.requestFrom(slave1Address, 1);

  while (Wire.available()) {
    receivedValueSlave1 = Wire.read();
    Serial.print("Read from Slave1 -> ");
    Serial.println(receivedValueSlave1);
  }

  // Process Adafruit IO events
  io.run();

  // Save ADC pot value to the 'adcValue' feed on Adafruit IO
  Serial.print("Sending ADC value -> ");
  Serial.println(adcValue);
  adcMasterFeed->save(adcValue);

  delay(2000);
  // Save slave pot value to the 'slaveValue' feed on Adafruit IO
  Serial.print("Sending Slave 1 value -> ");
  Serial.println(receivedValueSlave1);
  adcSlave1Feed->save(receivedValueSlave1);

  // Adafruit IO is rate limited for publishing, so add a delay between feed->save events
  delay(2000);
}

// This function is called whenever a 'led' message is received from Adafruit IO
void handleMessage(AdafruitIO_Data *data) {
  char receivedAdafruitData = *data->value();

  Serial.print("Received <- ");
  Serial.println(receivedAdafruitData);

  ledState = (receivedAdafruitData == '1');
}
