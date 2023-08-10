#include <Wire.h>

/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "Tumanita"
#define IO_KEY       "aio_CwuD60KvDT6YYqUS8xDM84EK3hJf"

/******************************* WIFI **************************************/

//#define WIFI_SSID "DinisTumaninis"
//#define WIFI_PASS "LuluDini123@"
#define WIFI_SSID "Doggy"
#define WIFI_PASS "Doggy.2021"

#include "AdafruitIO_WiFi.h"


AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
