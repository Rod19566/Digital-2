/*
 * ////////////////////////////////
Proyecto final
Dina Rodr�guez      19566
Este programa va en el TIVA C y est� encargado
de encender y apagar las luces del Park-o-Meter
//////////////////////////
 * */



#include <stdint.h>
#include <stdbool.h>

#include "inc/tm4c123ge6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"


extern void Timer0IntHandler(void);
extern void UARTIntHandler(void);

uint32_t ui32Period;

unsigned char l = 'a';
int parqueos[] = {0,0,0,0};
int taken = 0;

/**
 * main.c
 */


/*
 * instancias de los voids
 *
 */
int available(int a[],int n);

////////////////

/*
 * Created voids
 * */
int available(int a[],int n)
{
   int i,sum=0;

   for(i=0; i<n; i++)
   {
        sum+=a[i];

   }
   return sum;
}

///

void setup(void){

    ///////// CONFIGURACI�N DEL RELOJ ///////////////////////
        SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
         //CLK FREQUENCY 40 MHz
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //ENABLES PORT F FOR TIMER
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); //ENABLES TIMER0

    ////////////////////////////////////////////////////////////
         ///////// CONFIGURACI�N DE LOS PINES ///////////////////////



        ////////////////////////////////////////////////////////////
         //CONFIGURACION UART//////////////////////////
         SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //SE HABILITA PUERTO A
         SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //SE HABILITA EL UART
         UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
  //       GPIOPinConfigure(0x00000001); //RX
  //       GPIOPinConfigure(0x00000401); //TX
         UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
         GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
         UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
         IntEnable(INT_UART0);
         UARTEnable(UART0_BASE);

         UARTIntRegister(UART0_BASE, UARTIntHandler);
         IntMasterEnable();    // SE HABILITAN LAS INTERRUPCIONES GLOBALES
         ////////////////////////////////////////////////////////////



}
int main(void)
{
    setup();

     while(1){
         taken = available(parqueos[], 3);
         SysCtlDelay(10000000);       //delay

     }//while(1) "loop"

}//main.



void UARTIntHandler(void){

    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT); //SE LIMPIA EL UART




}//void UARTIntHandler


