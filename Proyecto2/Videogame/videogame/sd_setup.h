#ifndef _SD_SETUP_H_
#define _SD_SETUP_H_

#include <stdint.h>

//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void sd_Init(void);

void sd_Init(void){
  SPI.setModule(0);
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(12, OUTPUT);

  if (!SD.begin(12)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
}

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
