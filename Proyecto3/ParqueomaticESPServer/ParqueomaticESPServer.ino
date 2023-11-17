/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server con acceso a SPIFFS
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
  https://github.com/luisllamasbinaburo/ESP32-Examples
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
#include "SevSeg.h"
SevSeg sevseg;
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "DinisTumaninis";  // Enter your SSID here
const char* password = "LuluDini123@";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)
bool LED1status = LOW;
uint8_t LED1red = 2;
uint8_t LED1green = 4;

bool LED2status = LOW;
uint8_t LED2red = 17;
uint8_t LED2green = 5;

bool LED3status = LOW;
uint8_t LED3red = 19;
uint8_t LED3green = 21;

bool LED4status = LOW;
uint8_t LED4red = 22;
uint8_t LED4green = 23;

uint8_t availParkingLots = 4;

int sensor1 = 13;
int sensor2 = 12;
int sensor3 = 15;
int sensor4 = 16;
int onOff[] = {1,1,1,1};
int s = 0;
//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);

 ///////////////////7seg setup 
  byte numDigits = 1;
  byte digitPins[] = {};
  //byte segmentPins[] = {A, B, C, D, E, F, G, DP};
  byte segmentPins[] = {25, 18, 27, 14, 26, 33, 32, 16};
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_CATHODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(75);

/////////leds setup
  pinMode(LED1red, OUTPUT);
  pinMode(LED1green, OUTPUT);
  pinMode(LED2red, OUTPUT);
  pinMode(LED2green, OUTPUT);
  pinMode(LED3red, OUTPUT);
  pinMode(LED3green, OUTPUT);
  pinMode(LED4red, OUTPUT);
  pinMode(LED4green, OUTPUT);
  
  pinMode(sensor1, INPUT_PULLUP);// define pin two as input for dipswitch
  pinMode(sensor2, INPUT_PULLUP);// define pin two as input for dipswitch
  pinMode(sensor3, INPUT_PULLUP);// define pin two as input for dipswitch
  pinMode(sensor4, INPUT_PULLUP);// define pin two as input for dipswitch

///////////////server setup
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

//////////https handlers
  server.on("/", handle_OnConnect); // Directly
  server.on("/lot1On", handle_lot1On);
  server.on("/lot1Off", handle_lot1Off);
  server.on("/lot2On", handle_lot2On);
  server.on("/lot2Off", handle_lot2Off);
  server.on("/lot3On", handle_lot3On);
  server.on("/lot3Off", handle_lot3Off);
  server.on("/lot4On", handle_lot4On);
  server.on("/lot4Off", handle_lot4Off);
  server.on("/refresh", handle_refresh);
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  s = 0;
  for (int i=0; i < 4; i++)
  {
      s += onOff[i];
  }
  sevseg.setNumber(s);
  //sevseg.setNumber(availParkingLots);
  int pushed1 = digitalRead(sensor1);// read pin 2 and put the result in the "pusshed" variable
  int pushed2 = digitalRead(sensor2);// read pin 2 and put the result in the "pusshed" variable
  int pushed3 = digitalRead(sensor3);// read pin 2 and put the result in the "pusshed" variable
  int pushed4 = digitalRead(sensor4);// read pin 2 and put the result in the "pusshed" variable

  sevseg.refreshDisplay();
    ///////////////// LED 1 ////////////////////////
  if (pushed1) //LED4status
  { 
    LED4status = LOW;
    onOff[0] = 1;
    digitalWrite(LED1red, HIGH);
    digitalWrite(LED1green, LOW);
    // handle_lot4Off();
  }
  else
  {
    LED4status = HIGH;
    onOff[0] = 0;
    digitalWrite(LED1red, LOW);
    digitalWrite(LED1green, HIGH);
    // handle_lot4On();
  }
    ///////////////// LED 2 ////////////////////////
  if (pushed2) //LED3status)
  {
    LED3status = LOW;
    onOff[1] = 1;
    digitalWrite(LED2red, HIGH);
    digitalWrite(LED2green, LOW);
    // handle_lot3Off();
  }
  else
  {
    LED3status = HIGH;
    onOff[1] = 0;
    digitalWrite(LED2red, LOW);
    digitalWrite(LED2green, HIGH);
    // handle_lot3On();
  }
  ////////////// LED 3 //////////////////////
  if (pushed3) //LED2status)
  {
    LED2status = LOW;
    onOff[2] = 1;
    digitalWrite(LED3red, HIGH);
    digitalWrite(LED3green, LOW);
    // handle_lot2Off();
  }
  else
  {
    LED2status = HIGH;
    onOff[2] = 0;
    digitalWrite(LED3red, LOW);
    digitalWrite(LED3green, HIGH);
    // handle_lot2On();
  } 
  ///////////////// LED 4 ////////////////////////
  if (pushed4) //LED1status)
  {
    LED1status = LOW;
    onOff[3] = 1;
    digitalWrite(LED4red, HIGH);
    digitalWrite(LED4green, LOW);
    // handle_lot1Off();
  }
  else
  {
    LED1status = HIGH;
    onOff[3] = 0;
    digitalWrite(LED4red, LOW);
    digitalWrite(LED4green, HIGH);
    // handle_lot1On();
  }
  delay(300);
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  LED4status = LOW;
  Serial.println("GPIO2 Status: OFF");
  Serial.println("GPIO3 Status: OFF");
  Serial.println("GPIO4 Status: OFF");
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
//************************************************************************************************
// Handlers to turn on Led Statuses
//************************************************************************************************
void handle_lot1On() {
  LED1status = HIGH;
  availParkingLots--;
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
void handle_lot2On() {
  LED2status = HIGH;
  availParkingLots--;
  Serial.println("GPIO3 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
void handle_lot3On() {
  LED3status = HIGH;
  availParkingLots--;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
void handle_lot4On() {
  LED4status = HIGH;
  availParkingLots--;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
void handle_refresh() {
  Serial.println("Refreshing");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
//************************************************************************************************
// Handler de lot1Off
//************************************************************************************************
void handle_lot1Off() {
  LED1status = LOW;
  availParkingLots++;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
void handle_lot2Off() {
  LED2status = LOW;
  availParkingLots++;
  Serial.println("GPIO3 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
void handle_lot3Off() {
  LED3status = LOW;
  availParkingLots++;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
void handle_lot4Off() {
  LED4status = LOW;
  availParkingLots++;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, s));
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t parking1, uint8_t parking2, uint8_t parking3, uint8_t parking4, int free) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no, http-equiv=\"refresh\" content=10\">\n";
  ptr += "<title>Parqueomatic</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n"; 
  ptr += ".parking-space {display: inline-block;margin: 20px;}\n";
  ptr += ".circle {width: 50px;height: 50px;border-radius: 50%;display: inline-block;margin-bottom: 10px;}\n";
  ptr += ".circle-off {background-color: #e74c3c;}\n";
  ptr += ".circle-on {background-color: #2ecc71;}\n";    
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Parqueomatic &#128664</h1>\n";
  ptr += "<h3>Dina Rodriguez</h3>\n";

  if (parking1){  
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 1:</p>\n";
    ptr += "<div class=\"circle circle-off\"></div>\n";
    // ptr += "<a class=\"button button-off\" href=\"/lot1Off\">Busy</a>\n";
    ptr += "</div>\n";
  }
  else{
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 1:</p>\n";
    ptr += "<div class=\"circle circle-on\"></div>\n";
    // ptr += "<a class=\"button button-on\" href=\"/lot1On\">Free</a>\n";
    ptr += "</div>\n";
  }

  if (parking2){
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 2:</p>\n";
    ptr += "<div class=\"circle circle-off\"></div>\n";
    // ptr += "<a class=\"button button-off\" href=\"/lot2Off\">Busy</a>\n";
    ptr += "</div>\n";
  }
  else{
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 2:</p>\n";
    ptr += "<div class=\"circle circle-on\"></div>\n";
    // ptr += "<a class=\"button button-on\" href=\"/lot2On\">Free</a>\n";
    ptr += "</div>\n";
  }
  if (parking3){
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 3:</p>\n";
    ptr += "<div class=\"circle circle-off\"></div>\n";
    // ptr += "<a class=\"button button-off\" href=\"/lot3Off\">Busy</a>\n";
    ptr += "</div>\n";
  }
  else{
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 3:</p>\n";
    ptr += "<div class=\"circle circle-on\"></div>\n";
    // ptr += "<a class=\"button button-on\" href=\"/lot3On\">Free</a>\n";
    ptr += "</div>\n";
  }
  
  if (parking4){
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 4:</p>\n";
    ptr += "<div class=\"circle circle-off\"></div>\n";
    // ptr += "<a class=\"button button-off\" href=\"/lot4Off\">Busy</a>\n";
    ptr += "</div>\n";
  }
  else{
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 4:</p>\n";
    ptr += "<div class=\"circle circle-on\"></div>\n";
    //ptr += "<a class=\"button button-on\" href=\"/lot4On\">Free</a>\n";
    ptr += "</div>\n";
  }
  ptr += "<p>Available Parking Lots: ";
  switch(free){
    case 0: ptr += "0</p>\n";
    break;
    case 1: ptr += "1</p>\n";
    break;
    case 2: ptr += "2</p>\n";
    break;
    case 3: ptr += "3</p>\n";
    break;
    case 4: ptr += "4</p>\n";
    break;
  }
  ptr += "<a class=\"button button-on\" href=\"/refresh\">Refresh</a>\n";

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}
//************************************************************************************************
// Obtener el tipo de contenido del archivo
//************************************************************************************************
String GetContentType(String filename)
{
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

