/*
 * File:   lab05_main.c
 * Author: angel
 *
 * Created on 16 de agosto de 2021, 01:36 AM
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
#define  _XTAL_FREQ 8000000

///////////////// declaración de variables /////////////////////////////////////

uint8_t cantidad; 
char cont[3]; 
unsigned char i;
unsigned char seleccion;
unsigned char bloqueo = 1; 
uint8_t temperatura; 

//////////////////////// subrutina de interrupcion /////////////////////////////

void __interrupt()isr(void){
    
    // interrucion por medio de bandera del ON CHANGE.
    if (RBIF == 1){
        if (RB0 == 0) {
            if (PORTD == 0xff) {
                PORTD = 0x00;        // condiciones para que al hacer onderflow 
            }                        // pase el contado a 0
            else {
              PORTD++;               // incremento el puerto o contador
            }
        }
        else if (RB1 == 0) {
            if (PORTD == 0x00) {
                PORTD = 0xff;        // condiciones para que al hacer underflow
            }                        // pase el contado a 15
            else {
              PORTD--;               // decremento el puerto. 
            }
        } 
    cantidad = PORTD;                // paso lo que esta en PORTD a la variable
    INTCONbits.RBIF = 0; // luego de todo esto, bajo la bandera de ON CHANGE.
    }
    // interrucion por recepcion de informacion USART 
    if (RCIF == 1) {
        seleccion = RCREG;        // lo que reciba de la PC lo pongo en la variable
        RCIF = 0;             // bajo la bandera
        bloqueo = 0; 
    }
    return; 
}

void main(void) {
    
///////////////////// configuraciones de pines del pic /////////////////////////
 
ANSEL = 0x00; 
ANSELH = 0x00;

TRISA = 0x00; 
TRISB = 0b00000011; 
TRISD = 0x00;
TRISE = 0x00;

PORTA = 0x00;
PORTB = 0x00;
PORTC = 0x00;
PORTD = 0x00;
PORTE = 0x00;

// configuracion de PULL UP internos en PORTB///////////////////////////////////

OPTION_REGbits.nRBPU = 0;       
IOCBbits.IOCB0 = 1;        // configuracion de los PULL UP internos en los
IOCBbits.IOCB1 = 1;        // pines RB0 y RB1
WPUB = 0b00000011;

//////////////////////// configuracion del oscilador interno ///////////////////

/* en este caso voy a configurarlo para que opere a 8 Mhz por lo que sera la 
 opcion 8 que le ingrese a la funcion de la libreria. */

conf_OSCCON(8);

//////////////// configuracion de comunicacion USART  //////////////////////////

conf_USART();

/*  habilitando las banderas de interrupcion para USART y ON CHANGE*/

INTCONbits.GIE = 1;
INTCONbits.RBIE = 1;
INTCONbits.RBIF = 0;  // interrupciones del ON CHANGE 
INTCONbits.PEIE = 1;
PIE1bits.RCIE = 1; 
PIR1bits.RCIF = 0;   // USART

////////////////////////////// LOOP principal //////////////////////////////////

while (1) {
    
    while (bloqueo == 1) ; // me quedo quieto hasta deshabilitar el bloqueo osea
    // cuando el usuario escriba una de las elecciones 
    bloqueo = 1;
    
    switch (seleccion) {
        case('a'):
            cont[0] = centenas(cantidad);
            cont[1] = decenas(cantidad);  // obtengo en caracteres el valor 
            cont[2] = unidades(cantidad); // decimal del contador del PORTD
            
            for (i = 0; i < strlen(cont); i++) {
                __delay_ms(100);
                if (TXIF == 1) {          // despliego el mensaje del menu 
                    TXREG = cont[i];
                }
            }
        break;
        
        case('b'):
            while (bloqueo == 1) ; // me quedo quieto hasta deshabilitar el
            // bloqueo osea cuando el usuario escriba una de las elecciones 
            PORTA = RCREG;  // recibo los datos y los paso a la variable "PORTA"
            bloqueo = 1;
        break;
    }
 
}
return; 
}