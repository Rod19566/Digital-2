#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

//////////////////////////////PROTOTYPES////////////////////////////

void UART1config(void);

/**
 * main.c
 */

//////////////////////////////VARIABLES////////////////////////////
uint8_t parking1;
uint8_t parking2;
uint8_t parking3;
uint8_t parking4;

void main(void)
//////////////////////////////SETUP////////////////////////////
{   //Clock set to 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    //PORTS E, C y A enabled
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //LED pins are set as OUTPUTS
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);
    //Pins are configures as INPUTS
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3);
    //IntMasterEnable();
    UART1config();

    // Se manda mensajes por UART
    UARTCharPut(UART1_BASE, 'H');
    UARTCharPut(UART1_BASE, 'o');
    UARTCharPut(UART1_BASE, 'l');
    UARTCharPut(UART1_BASE, 'a');
/////////////////////////////MAIN LOOP////////////////////
    while(1){
        //Buttons are read
        parking1 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2);
        parking2 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3);
        parking3 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6);
        parking4 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7);

        //parking 1

        if (parking1 == 0){ //parking 1 is busy
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5, 16);   //RED LED ON
        }
        else{               //parking 1 is free
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5, 32);   //GREEN LED ON
        }

        //parking 2
        if (parking2 == 0){ //parking 2 is busy
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3, 4);
        }
        else{               //parking 2  is free
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3, 8);   //GREEN LED ON
        }

        //parking 3
        if (parking3 == 0){ //parking 3 is busy
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5, 16);   //REDLED ON
        }
        else{               //parking 3  is free
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5, 32);   //GREEN LED ON
        }

        //parking 4
        if (parking4 == 0){ //parking 4 is busy
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7, 64);   //REDLED ON
        }
        else{               //parking 4 is free
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7, 128);   //GREEN LED ON
            }

        UARTCharPut(UART1_BASE, 'P');            //ESP32 communication
        UARTCharPut(UART1_BASE, parking1);      //ESP32 communication
        UARTCharPut(UART1_BASE, parking2);      //ESP32 communication
        UARTCharPut(UART1_BASE, parking3);      //ESP32 communication
        UARTCharPut(UART1_BASE, parking4);      //ESP32 communication

}
}

////////////////////////////FUNCTIONS////////////////////////////
void UART1config(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);    //UART1 clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);    //PORTC clock
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5); //PORTC pin 4 and 5 enabled RX and TX
    UARTConfigSetExpClk(UART1_BASE,SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTEnable(UART1_BASE);

    /*
   // UARTIntEnable(UART2_BASE, UART_INT_RT | UART_INT_RX); //Interruptions
   // UARTIntRegister(UART2_BASE, UARTIntHandler);  //handler name
    */
}

