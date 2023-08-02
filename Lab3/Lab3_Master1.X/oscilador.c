
#include "oscillator.h"

    void configOsc(uint16_t frec){
        if (frec == 8) OSCCONbits.IRCF = 0b111;
        if (frec == 4) OSCCONbits.IRCF = 0b110;
        if (frec == 1) OSCCONbits.IRCF = 0b100;
        if (frec == 500) OSCCONbits.IRCF = 0b011;
        if (frec == 250) OSCCONbits.IRCF = 0b101;
        if (frec == 125) OSCCONbits.IRCF = 0b001;
        if (frec == 31) OSCCONbits.IRCF = 0b000;
        OSCCONbits.SCS = 1;
    }

