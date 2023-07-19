#ifndef SETUP_H
#define SETUP_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void setupF(void);
void ioc_init (char pin);
void buttonPressed(void);
void upButtonF(void);
void downButtonF(void);

#endif //setup



