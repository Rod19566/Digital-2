// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

#include "config.h"

const int potPin = 34;

// this int will hold the current count for our sketch

#define ledPin 2

int adcValue = 0;
boolean ledState = false;

// set up the 'Example1' feed
AdafruitIO_Feed *adcFeed = io.feed("example1");
AdafruitIO_Feed *ledFeed = io.feed("led");

void setup() {

  // start the serial connection
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,LOW);

  // wait for serial monitor to open
  while(!Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  ledFeed->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }  
  //ledFeed->get();

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  int adcValue = analogRead(potPin);
  adcValue = map(adcValue, 0, 4095, 0, 255);

  if (ledState) digitalWrite(ledPin,HIGH);
  else digitalWrite(ledPin,LOW);

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

 // adcValue = random(0,100);

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(adcValue);
  adcFeed->save(adcValue);

  // increment the count by 1
  //adcValue++;

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) { //once feed receives message
  char receivedAdafruitData = *data->value();

  Serial.print("received <- ");
  Serial.println(receivedAdafruitData);

  if (receivedAdafruitData == '1') ledState = true;
  else ledState = false;

}