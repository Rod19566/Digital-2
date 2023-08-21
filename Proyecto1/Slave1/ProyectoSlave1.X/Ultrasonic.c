/*
 * File:   Ultrasonic.c
 * Author: WMGWW
 *
 * Created on 19 de agosto de 2023, 11:29 AM
 */
#include "Ultrasonic.h"

void timer1Config(void){
    //T1CONbits.RD16=1;
    T1CONbits.T1CKPS0 = 0b00;
    T1CONbits.TMR1CS = 0;
    TMR1 = 0;
    TMR1ON = 0;
}

uint16_t getDistance(){
    uint16_t Duration;
    uint16_t Distance;
    uint16_t Timer1;
    uint16_t TRIGGER = 1;
    uint16_t ECHO = 1;
    TRIGGER = 1;
    __delay_ms(10);
    TRIGGER = 0;
    while(ECHO == 0);
    T1CONbits.TMR1ON = 1;
    while(ECHO == 1);
    T1CONbits.TMR1ON = 0;
    Timer1 = TMR1;
    Duration = Timer1/2;
    if(Duration <= 23200)//400 cms de maximos del sensor si se usara float se usa el valor exacto 23529.4
    {
        Distance = Duration/58;
    }
    else if(Duration < 116)//rango minimo de 2 cm
        
    {
        Distance = 0;
    }
     else 
        
    {
        Distance = 0;
    }
    Duration = 0;
    TMR1 = 0;
    return Distance;
    
}