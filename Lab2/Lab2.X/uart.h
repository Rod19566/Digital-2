#ifndef UART_H
#define UART_H

#include <xc.h> // include processor files - each processor file is guarded.  

// PROTOTIPOS
void setupUart(void);
void UART_RX_config (uint16_t baudrate);
void UART_TX_config (uint16_t baudrate);
void UART_write_char(char c);
void UART_write_string(char* c);
unsigned char UART_read_char(unsigned char address);

#endif //interrupt




