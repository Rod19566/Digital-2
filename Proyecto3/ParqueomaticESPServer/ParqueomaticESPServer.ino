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
const char* ssid = "Luzelin";  // Enter your SSID here
const char* password = "Frijolina2147@";  //Enter your Password here

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

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);

 ///////////////////7seg setup 
  byte numDigits = 1;
  byte digitPins[] = {};
  //byte segmentPins[] = {6, 5, 2, 3, 4, 7, 8, 9};
  byte segmentPins[] = {25, 26, 12, 14, 27, 33, 32, 13};
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
  sevseg.setNumber(availParkingLots);
  sevseg.refreshDisplay();
    ///////////////// LED 1 ////////////////////////
  if (!LED4status)
  {
    digitalWrite(LED1red, HIGH);
    digitalWrite(LED1green, LOW);
  }
  else
  {
    digitalWrite(LED1red, LOW);
    digitalWrite(LED1green, HIGH);
  }
    ///////////////// LED 2 ////////////////////////
  if (!LED3status)
  {
    digitalWrite(LED2red, HIGH);
    digitalWrite(LED2green, LOW);
  }
  else
  {
    digitalWrite(LED2red, LOW);
    digitalWrite(LED2green, HIGH);
  }
  ////////////// LED 3 //////////////////////
  if (!LED2status)
  {
    digitalWrite(LED3red, HIGH);
    digitalWrite(LED3green, LOW);
  }
  else
  {
    digitalWrite(LED3red, LOW);
    digitalWrite(LED3green, HIGH);
  } 
  ///////////////// LED 4 ////////////////////////
  if (!LED1status)
  {
    digitalWrite(LED4red, HIGH);
    digitalWrite(LED4green, LOW);
  }
  else
  {
    digitalWrite(LED4red, LOW);
    digitalWrite(LED4green, HIGH);
  }
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
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status));
}
//************************************************************************************************
// Handlers to turn on Led Statuses
//************************************************************************************************
void handle_lot1On() {
  LED1status = HIGH;
  availParkingLots--;
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status));
}
void handle_lot2On() {
  LED2status = HIGH;
  availParkingLots--;
  Serial.println("GPIO3 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status));
}
void handle_lot3On() {
  LED3status = HIGH;
  availParkingLots--;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status));
}
void handle_lot4On() {
  LED4status = HIGH;
  availParkingLots--;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status));
}
//************************************************************************************************
// Handler de lot1Off
//************************************************************************************************
void handle_lot1Off() {
  LED1status = LOW;
  availParkingLots++;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status));
}
void handle_lot2Off() {
  LED2status = LOW;
  availParkingLots++;
  Serial.println("GPIO3 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status));
}
void handle_lot3Off() {
  LED3status = LOW;
  availParkingLots++;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status));
}
void handle_lot4Off() {
  LED4status = LOW;
  availParkingLots++;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status));
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t parking1, uint8_t parking2, uint8_t parking3, uint8_t parking4) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
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
    ptr += "<a class=\"button button-off\" href=\"/lot1Off\">Busy</a>\n";
    ptr += "</div>\n";
  }
  else{
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 1:</p>\n";
    ptr += "<div class=\"circle circle-on\"></div>\n";
    ptr += "<a class=\"button button-on\" href=\"/lot1On\">Free</a>\n";
    ptr += "</div>\n";
  }

  if (parking2){
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 2:</p>\n";
    ptr += "<div class=\"circle circle-off\"></div>\n";
    ptr += "<a class=\"button button-off\" href=\"/lot2Off\">Busy</a>\n";
    ptr += "</div>\n";
  }
  else{
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 2:</p>\n";
    ptr += "<div class=\"circle circle-on\"></div>\n";
    ptr += "<a class=\"button button-on\" href=\"/lot2On\">Free</a>\n";
    ptr += "</div>\n";
  }
  if (parking3){
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 3:</p>\n";
    ptr += "<div class=\"circle circle-off\"></div>\n";
    ptr += "<a class=\"button button-off\" href=\"/lot3Off\">Busy</a>\n";
    ptr += "</div>\n";
  }
  else{
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 3:</p>\n";
    ptr += "<div class=\"circle circle-on\"></div>\n";
    ptr += "<a class=\"button button-on\" href=\"/lot3On\">Free</a>\n";
    ptr += "</div>\n";
  }
  
  if (parking4){
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 4:</p>\n";
    ptr += "<div class=\"circle circle-off\"></div>\n";
    ptr += "<a class=\"button button-off\" href=\"/lot4Off\">Busy</a>\n";
    ptr += "</div>\n";
  }
  else{
    ptr += "<div class=\"parking-space\">\n";
    ptr += "<p>Parking Lot 4:</p>\n";
    ptr += "<div class=\"circle circle-on\"></div>\n";
    ptr += "<a class=\"button button-on\" href=\"/lot4On\">Free</a>\n";
    ptr += "</div>\n";
  }

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

