/*
 * File:   lab03_maestro.c
 * Author: angel
 *
 * Created on 2 de agosto de 2021, 08:16 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "libreria.h"
#include "SPI.h"
#define  _XTAL_FREQ 8000000

///////////////// declaración de variables /////////////////////////////////////

unsigned char canal = 0;
float conv1;
float conv2;
char voltaje1[4] ;
char voltaje2[4] ;
unsigned char i = 0; 

void main(void) {
    
///////////////////// configuraciones de pines del pic /////////////////////////
 
ANSEL = 0;
ANSELH = 0;
TRISE0 = 0;
TRISB = 0;
TRISD = 0;
PORTB = 0;
PORTD = 0;

PORTEbits.RE0 = 1;   // master select

//////////////////////// configuracion del oscilador interno ///////////////////

/* en este caso voy a configurarlo para que opere a 4 Mhz por lo que sera la 
 opcion 7 que le ingrese a la funcion de la libreria. */

conf_OSCCON(8);

//////////////// configuracion de comunicacion ISP  ////////////////////////////

spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

//////////////// configuracion de comunicacion USART  //////////////////////////

conf_USART();

/*  habilitando las banderas de interrupcion para USART*/

INTCONbits.GIE = 1;
INTCONbits.PEIE = 1;
PIE1bits.RCIE = 1; 
PIR1bits.RCIF = 0;

////////////////////////////// LOOP principal //////////////////////////////////

while (1) {
    
    while (RCIF == 0) ; /* me quedo quieto hasta recibir una señal para hacer la 
                     * recepcion de datos*/ 

    PORTB = RCREG;   // recibo los datos y los paso a la variable "PORTB"
    
    switch (canal) {
        case(0):
            
            PORTEbits.RE0 = 0;       //Slave Select
            __delay_ms(1);       
            spiWrite(canal);
            conv1 = (0.01961)*(spiRead()) ;
            __delay_ms(1);
            PORTEbits.RE0 = 1;       //Slave Deselect 
            __delay_ms(250);
            
            sprintf(voltaje1, "%3.2f", conv1);  // paso a caracteres el voltaje
            
            for (i = 0; i <= strlen(voltaje1); i++) {
                __delay_ms(100);
                if (TXIF == 1) {          // despliego el mensaje
                    TXREG = voltaje1[i];
                }
            }
    
            TXREG = 13;        // hago dos saltos de linea para tener un espacio
            __delay_ms(100);   // vacio entre cada mensaje
            TXREG = '\r';
            
        break;
        case(1):
            PORTEbits.RE0 = 0;       //Slave Select
            __delay_ms(1);       
            spiWrite(canal);
            conv2 = (0.01961)*(spiRead()) ;
            __delay_ms(1);
            PORTEbits.RE0 = 1;       //Slave Deselect 
            __delay_ms(250);
            
            sprintf(voltaje2, "%3.2f", conv2);   // paso a caracteres el voltaje
            
            for (i = 0; i <= strlen(voltaje2); i++) {
                __delay_ms(100);
                if (TXIF == 1) {          // despliego el mensaje
                    TXREG = voltaje2[i];
                }
            }
    
            TXREG = 13;        // hago dos saltos de linea para tener un espacio
            __delay_ms(100);   // vacio entre cada mensaje
            TXREG = '\r';
        break;
    }
    
    if (canal == 0) {
        canal = 1;          // intercambio el valor de canal para obtener ambas
    }                       // lecturas de los potenciometros en el esclavo. 
    else {
        canal = 0; 
    }
}
return;
}
