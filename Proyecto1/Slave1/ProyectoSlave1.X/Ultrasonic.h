#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define _XTAL_FREQ 8000000

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//Ultrasonic Sensor
#define TRIG RB2   //SALIDA 0
#define ECHO RB3   //ENTRADA 0
#define TRIG1 RB4   //SALIDA 1
#define ECHO1 RB5   //ENTRADA 1

void timer1Config(void);
uint16_t getDistance(int TRIGGER, int ECHO0);

#endif //oscilador



