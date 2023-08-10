#include <Wire.h>

/************************** Configuration ***********************************/

const int potPin = 18;
#define slave1Address 0x50

int adcRawValue = 0;
int adcValue = 0;

void setup() {
  Wire.begin(21, 22); // Use GPIO 21 for SDA and GPIO 22 for SCL
  Wire.onRequest(requestEvent);
  Serial.begin(115200);
}

void loop() {
  adcRawValue = analogRead(potPin);
  adcValue = map(adcRawValue, 0, 4095, 0, 255);
  // No need for additional code in the loop for the slave
  delay(100); // Add a delay for stability, adjust as needed
}

void requestEvent() {
  // Read analog value from the slave's potentiometer pin

  Serial.print("Sending ADC value -> ");
  Serial.println(adcValue);

  // Send the adcValue to the master
  Wire.write((byte *)&adcValue, sizeof(adcValue));
}
