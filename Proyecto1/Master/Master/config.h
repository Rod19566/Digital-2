#include <Wire.h>

/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "Tumanita"
#define IO_KEY       "aio_Mnbg34E2tS23tvgKfg8UycqcAXcM"

/******************************* WIFI **************************************/
//Phone
//#define WIFI_SSID "DinisTumaninis"
//#define WIFI_PASS "LuluDini123@"

//Home
#define WIFI_SSID "Doggy"
#define WIFI_PASS "Doggy.2021"

//House #2
//#define WIFI_SSID "CLARO1_DBDDE4"
//#define WIFI_PASS "Vo24gsRFpM"

#include "AdafruitIO_WiFi.h"


AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
